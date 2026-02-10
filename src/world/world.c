#include "world/world.h"
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <string.h>
#include "camera/camera.h"
#include "editor/conn_mgr.h"
#include "logic/gate.h"
#include "logic/wire.h"
#include "quadtree/quadtree.h"
#include "settings.h"

World CreateWorld() {
    World world = {0};

    world.userCamera = CreateUserCamera(Vector2Zero());

    world.quadtree = CreateQuadTree(GetUserCameraRect(world.userCamera));

    world.visibleCount = 0;

    world.gatesCapacity = 16;
    world.gatesSize = 0;
    world.gates = malloc(sizeof(Gate) * world.gatesCapacity);

    // preallocate QTEntity pool
    world.gateEntities = malloc(sizeof(QTEntity) * 1024); // max gates
    memset(world.gateEntities, 0, sizeof(QTEntity) * 1024);

    world.chipsCapacity = 16;
    world.chipsSize = 0;
    world.chips = malloc(sizeof(Chip) * world.chipsCapacity);

    return world;
}

void UpdateWorld(World *world, float deltaTime) {
    UpdateUserCamera(&world->userCamera, deltaTime);

    world->visibleCount = 0;

    QueryQuadTree(
        world->quadtree,
        GetUserCameraRect(world->userCamera),
        world->visibleEntities,
        &world->visibleCount,
        MAX_VISIBLE // added limit
    );
}

void DrawWorld(World* world, ConnMgr* connMgr) {
    BeginUserCameraMode(world->userCamera);

    for (int i = 0; i < world->visibleCount; i++)
        DrawGate(&world->gates[world->visibleEntities[i]->index]);

    for (int i = 0; i < connMgr->wireCount; i++) {
        DrawWire(&connMgr->wires[i]);
    }

    DrawQuadTree(world->quadtree);

    EndUserCameraMode();
}

void UnloadWorld(World *world) {
    if (!world) return;

    DestroyQuadTree(world->quadtree);
    free(world->gates);
    free(world->gateEntities);
    free(world->chips);
}

void AddChip(World* world, Chip chip) {
    if (world->chipsSize >= world->chipsCapacity) {
        world->chipsCapacity *= 2;
        world->chips = realloc(world->chips, sizeof(Chip) * world->chipsCapacity);
    }
    world->chips[world->chipsSize++] = chip;
}

void AddGate(World* world, Gate gate) {
    if (world->gatesSize >= world->gatesCapacity) {
        world->gatesCapacity *= 2;
        world->gates = realloc(world->gates, sizeof(Gate) * world->gatesCapacity);
    }

    int idx = world->gatesSize;
    world->gates[idx] = gate;

    // populate stable QTEntity from pool
    QTEntity* gateEntity = &world->gateEntities[idx];
    gateEntity->index = idx;
    gateEntity->rect = (Rectangle){ gate.position.x, gate.position.y, GATE_WIDTH, GATE_HEIGHT };
    gateEntity->type = QUADTREE_GATE;

    // expand quadtree if needed
    ExpandQuadTreeRoot(&world->quadtree, gateEntity->rect);
    InsertQuadTree(world->quadtree, gateEntity);

    world->gatesSize++;
}
