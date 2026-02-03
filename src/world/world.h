#pragma once

#include "logic/chip.h"
#include <stddef.h>
#include <raylib.h>

typedef struct World {
    RenderTexture viewport;
    int width;
    int height;

    Chip* chips;
    size_t chipsSize;
    int chipsCapacity;

    Gate* gates;
    size_t gatesSize;
    int gatesCapacity;
} World;

World CreateWorld();
void  UpdateWorld(World* world);
void  DrawWorld(World* world);
void  UnloadWorld(World* world);

void AddChip(World* world, Chip chip);
void AddGate(World* world, Gate gate); 