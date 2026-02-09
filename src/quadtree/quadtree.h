#pragma once
#include <raylib.h>

#define QUADTREE_CAPACITY 5

typedef enum QTEntityType {
    QUADTREE_GATE,
    QUADTREE_PORT,
    QUADTREE_CHIP,
} QuadTreeEntityType;

typedef struct QTEntity {
    QuadTreeEntityType type;
    void* entity; // pointer to whatever object this represents
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

// create a new quadtree with a given boundary
QuadTree CreateQuadTree(Rectangle boundary);

// insert an entity into a quadtree
int InsertQuadTree(QuadTree* qt, QTEntity* entity, Rectangle entityBounds);

// query entities inside a given range
void QueryQuadTree(QuadTree* qt, Rectangle range, QTEntity** found, int* foundCount);
