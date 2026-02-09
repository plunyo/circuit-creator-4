#include "world/world.h"
#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#include "logic/gate.h"
#include "quadtree/quadtree.h"
#include "settings.h"

World CreateWorld() {
    World world = { 0 };

    world.width = GetScreenWidth();
    world.height = GetScreenHeight() * WORLD_HEIGHT_MULTIPLIER;
    world.viewport = LoadRenderTexture(world.width, world.height);

    world.quadtree = CreateQuadTree((Rectangle){ 0.0f, 0.0f, world.width, world.height });

    world.gatesCapacity = 10;
    world.gatesSize = 0;
    world.gates = malloc(sizeof(Gate) * world.gatesCapacity);

    world.chipsCapacity = 10;
    world.chipsSize = 0;
    world.chips = malloc(sizeof(Chip) * world.chipsCapacity);

    return world;
}

void UpdateWorld(World *world) {
    if (IsWindowResized()) {
        world->width = GetScreenWidth();
        world->height = GetScreenHeight() * WORLD_HEIGHT_MULTIPLIER;
        world->viewport = LoadRenderTexture(world->width, world->height);
        world->quadtree = CreateQuadTree((Rectangle){ 0.0f, 0.0f, world->width, world->height });
    }

    
}

void DrawWorld(World* world) {
    BeginTextureMode(world->viewport);
        ClearBackground(GRAY);
        
        DrawRectangleLinesEx((Rectangle){ 0.0f, 0.0f, world->width, world->height }, 10.0f, DARKGRAY);
        
        
        Vector2 p1 = world->gates[0].output.position;
        Vector2 p4 = world->gates[1].inputs[0].position;            float dx = fabsf(p4.x - p1.x);
        
        
        float bend = fmaxf(dx * 0.5f, 100.0f);
        Vector2 c3 = (Vector2){ p4.x - bend, p4.y };
        Vector2 c2 = (Vector2){ p1.x + bend, p1.y };

        DrawSplineSegmentBezierCubic(p1, c2, c3, p4, GATE_PORT_RADIUS * 0.8f, DARKGRAY);
        
        for (int i = 0; i < world->gatesSize; i++) DrawGate(&world->gates[i]);
    EndTextureMode();

    DrawTextureRec(
        world->viewport.texture, 
        (Rectangle){ 0.0f, 0.0f, world->width, -world->height },
        (Vector2){ 0 },
        WHITE);
}

void UnloadWorld(World *world) {
    MemFree(world->chips);
    MemFree(world->gates);
    UnloadRenderTexture(world->viewport);
}

void AddChip(World* world, Chip chip) {
    if (world->chipsSize >= world->chipsCapacity) {
        world->chipsCapacity *= 2;
        world->chips = realloc(world->chips, sizeof(Chip) * world->chipsCapacity);
    }

    world->chips[world->chipsSize] = chip; // copy the struct
    world->chipsSize++;
}

void AddGate(World* world, Gate gate) {
    if (world->gatesSize >= world->gatesCapacity) {
        // grow the array
        world->gatesCapacity *= 2; // double the capacity
        world->gates = realloc(world->gates, sizeof(Gate) * world->gatesCapacity);
    }

    world->gates[world->gatesSize] = gate;
    world->gatesSize++;
}

