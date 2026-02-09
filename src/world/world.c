#include "world/world.h"
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <math.h>
#include "camera/camera.h"
#include "logic/gate.h"
#include "quadtree/quadtree.h"
#include "settings.h"

World CreateWorld() {
    World world = { 0 };

    world.userCamera = CreateUserCamera(Vector2Zero());

    world.quadtree = CreateQuadTree(GetUserCameraRect(world.userCamera));

    world.gatesCapacity = 10;
    world.gatesSize = 0;
    world.gates = malloc(sizeof(Gate) * world.gatesCapacity);

    world.chipsCapacity = 10;
    world.chipsSize = 0;
    world.chips = malloc(sizeof(Chip) * world.chipsCapacity);

    return world;
}

void UpdateWorld(World *world, float deltaTime) {
    UpdateUserCamera(&world->userCamera, GetFrameTime());
}

void DrawWorld(World* world) {
    BeginUserCameraMode(world->userCamera);
        if (world->gatesSize >= 2) {
            Vector2 p1 = world->gates[0].output.position;
            Vector2 p4 = world->gates[1].inputs[0].position;

            float dx = fabsf(p4.x - p1.x);
            float bend = fmaxf(dx * 0.5f, 100.0f);
            Vector2 c3 = (Vector2){ p4.x - bend, p4.y };
            Vector2 c2 = (Vector2){ p1.x + bend, p1.y };

            DrawSplineSegmentBezierCubic(p1, c2, c3, p4, GATE_PORT_RADIUS * 0.8f, DARKGRAY);
        }
        
        for (int i = 0; i < world->gatesSize; i++) DrawGate(&world->gates[i]);

        DrawQuadTree(world->quadtree);
    EndUserCameraMode();
}

void UnloadWorld(World *world) {
    MemFree(world->chips);
    MemFree(world->gates);
}

void AddChip(World* world, Chip chip) {
    if (world->chipsSize >= world->chipsCapacity) {
        world->chipsCapacity *= 2;
        world->chips = realloc(world->chips, sizeof(Chip) * world->chipsCapacity);
    }

    world->chips[world->chipsSize] = chip;
    world->chipsSize++;
}

void AddGate(World* world, Gate gate) {
    if (world->gatesSize >= world->gatesCapacity) {
        world->gatesCapacity *= 2;
        world->gates = realloc(world->gates, sizeof(Gate) * world->gatesCapacity);
    }

    world->gates[world->gatesSize] = gate;
    world->gatesSize++;

    Rectangle gateRect = (Rectangle){
        gate.position.x, gate.position.y,
        GATE_WIDTH, GATE_HEIGHT
    };

    QTEntity* gateEntity = malloc(sizeof(QTEntity));
    gateEntity->index    = world->gatesSize - 1;
    gateEntity->rect     = gateRect;
    gateEntity->type     = QUADTREE_GATE;

    ExpandQuadTreeRoot(&world->quadtree, gateRect);
    InsertQuadTree(world->quadtree, gateEntity);
}

