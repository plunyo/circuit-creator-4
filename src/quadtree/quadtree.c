#include "quadtree/quadtree.h"
#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include "utils/utils.h"

static void Subdivide(QuadTree* qt);

QuadTree* CreateQuadTree(Rectangle boundary) {
    QuadTree* qt = malloc(sizeof(QuadTree));
    if (!qt) return NULL;

    qt->boundary = boundary;
    qt->northeast = qt->northwest = qt->southeast = qt->southwest = NULL;
    qt->count = 0;
    memset(qt->entities, 0, sizeof(qt->entities));

    return qt;
}

static void Subdivide(QuadTree* qt) {
    if (!qt) return;

    float x = qt->boundary.x;
    float y = qt->boundary.y;
    float w = qt->boundary.width / 2.0f;
    float h = qt->boundary.height / 2.0f;

    qt->northwest = CreateQuadTree((Rectangle){ x,     y,     w, h });
    qt->northeast = CreateQuadTree((Rectangle){ x + w, y,     w, h });
    qt->southeast = CreateQuadTree((Rectangle){ x + w, y + h, w, h });
    qt->southwest = CreateQuadTree((Rectangle){ x,     y + h, w, h });

    if (!qt->northwest || !qt->northeast || !qt->southeast || !qt->southwest) {
        DestroyQuadTree(qt->northwest);
        DestroyQuadTree(qt->northeast);
        DestroyQuadTree(qt->southeast);
        DestroyQuadTree(qt->southwest);

        qt->northwest = qt->northeast = qt->southeast = qt->southwest = NULL;
        return;
    }

    QTEntity* temp[QUADTREE_CAPACITY];
    int tempCount = qt->count;
    for (int i = 0; i < tempCount; ++i) temp[i] = qt->entities[i];

    qt->count = 0;
    memset(qt->entities, 0, sizeof(qt->entities));

    for (int i = 0; i < tempCount; ++i) {
        QTEntity* ent = temp[i];
        if (RectangleContainsRect(qt->northeast->boundary, ent->rect) &&
            InsertQuadTree(qt->northeast, ent)) continue;

        if (RectangleContainsRect(qt->northwest->boundary, ent->rect) &&
            InsertQuadTree(qt->northwest, ent)) continue;

        if (RectangleContainsRect(qt->southeast->boundary, ent->rect) &&
            InsertQuadTree(qt->southeast, ent)) continue;

        if (RectangleContainsRect(qt->southwest->boundary, ent->rect) &&
            InsertQuadTree(qt->southwest, ent)) continue;

        if (qt->count < QUADTREE_CAPACITY) {
            qt->entities[qt->count++] = ent;
        }
    }
}

int InsertQuadTree(QuadTree* qt, QTEntity* entity) {
    if (!CheckCollisionRecs(qt->boundary, entity->rect))
        return 0;

    if (!qt->northeast && qt->count >= QUADTREE_CAPACITY) {
        Subdivide(qt);
    }

    if (qt->northeast) {
        if (qt->northeast && RectangleContainsRect(qt->northeast->boundary, entity->rect)) return InsertQuadTree(qt->northeast, entity);
        if (qt->northwest && RectangleContainsRect(qt->northwest->boundary, entity->rect)) return InsertQuadTree(qt->northwest, entity);
        if (qt->southeast && RectangleContainsRect(qt->southeast->boundary, entity->rect)) return InsertQuadTree(qt->southeast, entity);
        if (qt->southwest && RectangleContainsRect(qt->southwest->boundary, entity->rect)) return InsertQuadTree(qt->southwest, entity);
    }

    if (qt->count < QUADTREE_CAPACITY) {
        qt->entities[qt->count++] = entity;
        return 1;
    }

    return 0;
}

void QueryQuadTree(QuadTree* qt, Rectangle range, QTEntity** found, int* foundCount) {
    if (!qt || !CheckCollisionRecs(qt->boundary, range)) return;

    for (int i = 0; i < qt->count; i++) {
        if (CheckCollisionRecs(range, qt->entities[i]->rect)) {
            found[(*foundCount)++] = qt->entities[i];
        }
    }

    if (!qt->northeast) return;

    if (qt->northeast) QueryQuadTree(qt->northeast, range, found, foundCount);
    if (qt->northwest) QueryQuadTree(qt->northwest, range, found, foundCount);
    if (qt->southeast) QueryQuadTree(qt->southeast, range, found, foundCount);
    if (qt->southwest) QueryQuadTree(qt->southwest, range, found, foundCount);
}

// expand root one step at a time so the old root always perfectly matches a child quadrant
void ExpandQuadTreeRoot(QuadTree** root, Rectangle rectOutside) {
    if (!root || !*root) return;

    while (!RectangleContainsRect((*root)->boundary, rectOutside)) {
        QuadTree* oldRoot = *root;
        Rectangle ob = oldRoot->boundary;

        int expandLeft   = rectOutside.x < ob.x;
        int expandRight  = rectOutside.x + rectOutside.width > ob.x + ob.width;
        int expandTop    = rectOutside.y < ob.y;
        int expandBottom = rectOutside.y + rectOutside.height > ob.y + ob.height;

        // new boundary expands exactly by one oldRoot width/height in any direction needed
        float newX = ob.x - (expandLeft ? ob.width : 0.0f);
        float newY = ob.y - (expandTop  ? ob.height : 0.0f);
        float newW = ob.width  * ( (expandLeft || expandRight) ? 2.0f : 1.0f );
        float newH = ob.height * ( (expandTop  || expandBottom) ? 2.0f : 1.0f );

        Rectangle newBoundary = (Rectangle){ newX, newY, newW, newH };
        QuadTree* newRoot = CreateQuadTree(newBoundary);
        if (!newRoot) return;

        // child rectangles are exact halves of the new boundary
        float halfW = newW * 0.5f;
        float halfH = newH * 0.5f;

        Rectangle nwRect = (Rectangle){ newX,         newY,         halfW, halfH };
        Rectangle neRect = (Rectangle){ newX + halfW, newY,         halfW, halfH };
        Rectangle seRect = (Rectangle){ newX + halfW, newY + halfH, halfW, halfH };
        Rectangle swRect = (Rectangle){ newX,         newY + halfH, halfW, halfH };

        // place the old root in whichever quadrant its center falls into
        float oldCenterX = ob.x + ob.width * 0.5f;
        float oldCenterY = ob.y + ob.height * 0.5f;
        float midX = newX + halfW;
        float midY = newY + halfH;

        if (oldCenterX < midX && oldCenterY < midY) {
            newRoot->northwest = oldRoot;
            newRoot->northeast = CreateQuadTree(neRect);
            newRoot->southeast = CreateQuadTree(seRect);
            newRoot->southwest = CreateQuadTree(swRect);
        } else if (oldCenterX >= midX && oldCenterY < midY) {
            newRoot->northeast = oldRoot;
            newRoot->northwest = CreateQuadTree(nwRect);
            newRoot->southeast = CreateQuadTree(seRect);
            newRoot->southwest = CreateQuadTree(swRect);
        } else if (oldCenterX >= midX && oldCenterY >= midY) {
            newRoot->southeast = oldRoot;
            newRoot->northwest = CreateQuadTree(nwRect);
            newRoot->northeast = CreateQuadTree(neRect);
            newRoot->southwest = CreateQuadTree(swRect);
        } else {
            newRoot->southwest = oldRoot;
            newRoot->northwest = CreateQuadTree(nwRect);
            newRoot->northeast = CreateQuadTree(neRect);
            newRoot->southeast = CreateQuadTree(seRect);
        }

        *root = newRoot;
    }
}

void DrawQuadTree(QuadTree *qt) {
    if (!qt) return;

    DrawRectangleLinesEx(qt->boundary, 5.0f, ColorAlpha(SKYBLUE, 0.4f));

    DrawQuadTree(qt->northwest);
    DrawQuadTree(qt->northeast);
    DrawQuadTree(qt->southwest);
    DrawQuadTree(qt->southeast);
}

void DestroyQuadTree(QuadTree* qt) {
    if (!qt) return;

    DestroyQuadTree(qt->northeast);
    DestroyQuadTree(qt->northwest);
    DestroyQuadTree(qt->southeast);
    DestroyQuadTree(qt->southwest);

    free(qt);
}
