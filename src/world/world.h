#pragma once

#include "logic/chip.h"
#include "quadtree/quadtree.h"
#include <stddef.h>
#include <raylib.h>

#define MAX_VISIBLE 1024  // safe upper limit

typedef struct World {
    Camera2D userCamera;

    QuadTree* quadtree;
    QTEntity* visibleEntities[MAX_VISIBLE]; // static safe array
    int visibleCount;

    Chip* chips;
    size_t chipsSize;
    int chipsCapacity;

    Gate* gates;
    size_t gatesSize;
    int gatesCapacity;

    QTEntity* gateEntities; // pool for all gates
} World;

World CreateWorld();
void  UpdateWorld(World* world, float deltaTime);
void  DrawWorld(World* world);
void  UnloadWorld(World* world);

void AddChip(World* world, Chip chip);
void AddGate(World* world, Gate gate);
