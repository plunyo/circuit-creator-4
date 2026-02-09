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
    void*              entity  ;
} QTEntity;

typedef struct QuadTree {
    struct QuadTree* northeast;
    struct QuadTree* northwest;
    struct QuadTree* southeast;
    struct QuadTree* southwest;

    QTEntity* entities[QUADTREE_CAPACITY];
    int             count;
} QuadTree;

QuadTree CreateQuadTree(Rectangle boundary);
void     Insert()
void     QueryQuadTree(QuadTree *qt, Rectangle range, QTEntity **found, int *foundCount);

// get chatgpt to do .h and .c