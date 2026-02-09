#pragma once

#include <raylib.h>

#define QUADTREE_CAPACITY 3

typedef enum QTEntityType {
    QUADTREE_GATE,
    QUADTREE_PORT,
    QUADTREE_CHIP,
} QuadTreeEntityType;

typedef struct QTEntity {
    QuadTreeEntityType type;
    Rectangle rect;
    int       index;
} QTEntity;

typedef struct QuadTree {
    Rectangle boundary;                 // bounding box of this quad
    
    struct QuadTree* northeast;
    struct QuadTree* northwest;
    struct QuadTree* southeast;
    struct QuadTree* southwest;

    QTEntity* entities[QUADTREE_CAPACITY];
    int count;                          // current number of entities
} QuadTree;

QuadTree* CreateQuadTree(Rectangle boundary);
int InsertQuadTree(QuadTree* qt, QTEntity* entity);
void QueryQuadTree(QuadTree* qt, Rectangle range, QTEntity** found, int* foundCount);
void ExpandQuadTreeRoot(QuadTree** root, Rectangle rectOutside);
void DrawQuadTree(QuadTree* qt);
void DestroyQuadTree(QuadTree* qt);
