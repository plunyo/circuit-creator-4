#include "quadtree/quadtree.h"
#include <stdlib.h>
#include <string.h>

// check if rect a fully contains rect b
static int RectangleContainsRect(Rectangle a, Rectangle b) {
    return b.x >= a.x && b.x + b.width <= a.x + a.width &&
           b.y >= a.y && b.y + b.height <= a.y + a.height;
}

// check if two rectangles overlap
static int RectangleIntersects(Rectangle a, Rectangle b) {
    return !(b.x > a.x + a.width || 
             b.x + b.width < a.x || 
             b.y > a.y + a.height || 
             b.y + b.height < a.y);
}

QuadTree CreateQuadTree(Rectangle boundary) {
    QuadTree qt;
    qt.boundary = boundary;
    qt.northeast = qt.northwest = qt.southeast = qt.southwest = NULL;
    qt.count = 0;
    memset(qt.entities, 0, sizeof(qt.entities));
    return qt;
}

// subdivide a quadtree into 4 children
static void Subdivide(QuadTree* qt) {
    float x = qt->boundary.x;
    float y = qt->boundary.y;
    float w = qt->boundary.width / 2;
    float h = qt->boundary.height / 2;

    qt->northeast = malloc(sizeof(QuadTree));
    *qt->northeast = CreateQuadTree((Rectangle){x + w, y, w, h});

    qt->northwest = malloc(sizeof(QuadTree));
    *qt->northwest = CreateQuadTree((Rectangle){x, y, w, h});

    qt->southeast = malloc(sizeof(QuadTree));
    *qt->southeast = CreateQuadTree((Rectangle){x + w, y + h, w, h});

    qt->southwest = malloc(sizeof(QuadTree));
    *qt->southwest = CreateQuadTree((Rectangle){x, y + h, w, h});
}

// insert an entity using its bounds (Rectangle) instead of a single point
int InsertQuadTree(QuadTree* qt, QTEntity* entity, Rectangle entityBounds) {
    if (!RectangleIntersects(qt->boundary, entityBounds))
        return 0; // no overlap with this quad

    // if we have space and no children, store it here
    if (qt->count < QUADTREE_CAPACITY && !qt->northeast) {
        qt->entities[qt->count++] = entity;
        return 1;
    }

    // subdivide if needed
    if (!qt->northeast)
        Subdivide(qt);

    // try to insert into a child that fully contains the entity
    if (RectangleContainsRect(qt->northeast->boundary, entityBounds) &&
        InsertQuadTree(qt->northeast, entity, entityBounds)) return 1;

    if (RectangleContainsRect(qt->northwest->boundary, entityBounds) &&
        InsertQuadTree(qt->northwest, entity, entityBounds)) return 1;

    if (RectangleContainsRect(qt->southeast->boundary, entityBounds) &&
        InsertQuadTree(qt->southeast, entity, entityBounds)) return 1;

    if (RectangleContainsRect(qt->southwest->boundary, entityBounds) &&
        InsertQuadTree(qt->southwest, entity, entityBounds)) return 1;

    // if it doesn't fully fit anywhere, keep it in this node
    if (qt->count < QUADTREE_CAPACITY) {
        qt->entities[qt->count++] = entity;
        return 1;
    }

    // fallback: just insert here even if over capacity (ugly but safe)
    return 0;
}

// query entities inside a given range
void QueryQuadTree(QuadTree* qt, Rectangle range, QTEntity** found, int* foundCount) {
    if (!RectangleIntersects(qt->boundary, range))
        return;

    for (int i = 0; i < qt->count; i++) {
        Rectangle* entRect = (Rectangle*)qt->entities[i]->entity; // assumes entity points to Rectangle
        if (RectangleIntersects(range, *entRect)) {
            found[(*foundCount)++] = qt->entities[i];
        }
    }

    if (!qt->northeast) return;

    QueryQuadTree(qt->northeast, range, found, foundCount);
    QueryQuadTree(qt->northwest, range, found, foundCount);
    QueryQuadTree(qt->southeast, range, found, foundCount);
    QueryQuadTree(qt->southwest, range, found, foundCount);
}
