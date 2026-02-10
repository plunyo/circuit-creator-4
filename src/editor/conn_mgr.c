#include "editor/conn_mgr.h"
#include "logic/gate.h"
#include "logic/port.h"
#include "logic/wire.h"
#include "quadtree/quadtree.h"
#include "settings.h"
#include "utils/utils.h"
#include "world/world.h"
#include <raylib.h>
#include <stdlib.h>
#include <string.h>

#define SNAP_MULTIPLIER 3.0f

ConnMgr CreateConnMgr() {
    ConnMgr connMgr = { 0 };
    connMgr.from = NULL;
    connMgr.wireCapacity = 10;
    connMgr.wires = malloc(sizeof(Wire) * connMgr.wireCapacity);
    connMgr.wireCount = 0;
    return connMgr;
}

static float Vec2DistanceSq(Vector2 a, Vector2 b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return dx*dx + dy*dy;
}

static bool WireExists(ConnMgr* connMgr, Port* from, Port* to) {
    if (!connMgr) return false;
    for (int i = 0; i < connMgr->wireCount; ++i) {
        Wire* w = &connMgr->wires[i];
        if (w->from == from && w->to == to) return true;
    }
    return false;
}

// ---- helpers to find ports/gates ----
static Gate* FindGateContainingPort(World* world, Port* p) {
    if (!world || !p) return NULL;
    for (int i = 0; i < world->visibleCount; i++) {
        QTEntity* entity = world->visibleEntities[i];
        if (entity->type != QUADTREE_GATE) continue;
        Gate* gate = &world->gates[entity->index];
        if (&gate->output == p) return gate;
        // some gates (NOT) may have only 1 input, but safe to check up to 2
        for (int j = 0; j < 2; ++j) {
            if (&gate->inputs[j] == p) return gate;
        }
    }
    return NULL;
}

// ---- port-based nearest search ----
static Port* FindNearestInput(World* world, Vector2 worldPos, float snapRadius) {
    Port* best = NULL;
    float bestSq = snapRadius * snapRadius;

    for (int i = 0; i < world->visibleCount; i++) {
        QTEntity* entity = world->visibleEntities[i];
        if (entity->type != QUADTREE_GATE) continue;

        Gate* gate = &world->gates[entity->index];

        // loop over inputs (some gates like NOT may have 1 valid input but we store 2 slots)
        for (int j = 0; j < 2; j++) {
            Port* p = &gate->inputs[j];
            float d = Vec2DistanceSq(p->position, worldPos);
            if (d <= bestSq) {
                bestSq = d;
                best = p;
            }
        }
    }

    return best;
}

static Port* FindNearestOutput(World* world, Vector2 worldPos, float snapRadius) {
    Port* best = NULL;
    float bestSq = snapRadius * snapRadius;

    for (int i = 0; i < world->visibleCount; i++) {
        QTEntity* entity = world->visibleEntities[i];
        if (entity->type != QUADTREE_GATE) continue;

        Gate* gate = &world->gates[entity->index];

        Port* p = &gate->output;
        float d = Vec2DistanceSq(p->position, worldPos);
        if (d <= bestSq) {
            bestSq = d;
            best = p;
        }
    }

    return best;
}

// Try to connect depending on which kind of port `connMgr->from` is.
// If from is an output -> snap to nearest input (existing behaviour).
// If from is an input  -> snap to nearest output and create wire output->input.
static void TryConnectToNearestPort(ConnMgr* connMgr, World* world, Vector2 mousePos) {
    if (!connMgr || !connMgr->from || !world) return;

    float snapRadius = PORT_RADIUS * SNAP_MULTIPLIER;

    // find which gate the 'from' port belongs to and whether it's an output
    Gate* fromGate = FindGateContainingPort(world, connMgr->from);
    bool fromIsOutput = (fromGate && (&fromGate->output == connMgr->from));

    if (fromIsOutput) {
        // behave like before: output -> nearest input
        Port* target = FindNearestInput(world, mousePos, snapRadius);
        if (!target) return;
        if (target == connMgr->from) return;
        Gate* targetGate = FindGateContainingPort(world, target);
        // don't connect to same gate's input
        if (targetGate && targetGate == fromGate) return;
        if (WireExists(connMgr, connMgr->from, target)) return;

        AddWire(connMgr, CreateWire(connMgr->from, target));
    } else {
        // from is an input (or unknown). Try to find nearest output and create output->input
        Port* targetOut = FindNearestOutput(world, mousePos, snapRadius);
        if (!targetOut) return;
        if (targetOut == connMgr->from) return;
        Gate* targetGate = FindGateContainingPort(world, targetOut);
        // don't connect to same gate
        if (targetGate && targetGate == fromGate) return;
        // ensure we don't already have that wire (output->input)
        if (WireExists(connMgr, targetOut, connMgr->from)) return;

        AddWire(connMgr, CreateWire(targetOut, connMgr->from));
    }
}

static void HandleGateDrag(ConnMgr* connMgr, Gate* gate, Vector2 mousePos) {
    if (!gate) return;

    // check output
    Port* output = &gate->output;
    if (CheckCollisionPointCircle(mousePos, output->position, PORT_RADIUS)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            connMgr->from = output;
            return;
        }
    }

    // check inputs (allow starting from inputs as well)
    for (int i = 0; i < 2; ++i) {
        Port* input = &gate->inputs[i];
        if (CheckCollisionPointCircle(mousePos, input->position, PORT_RADIUS)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                connMgr->from = input;
                return;
            }
        }
    }
}

static void UpdateGates(ConnMgr* connMgr, World* world, Vector2 mousePos) {
    for (int i = 0; i < world->visibleCount; i++) {
        QTEntity* entity = world->visibleEntities[i];
        if (entity->type == QUADTREE_GATE) {
            Gate* gate = &world->gates[entity->index];
            HandleGateDrag(connMgr, gate, mousePos);
        }
    }
}

void UpdateConnMgr(ConnMgr* connMgr, World* world) {
    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), world->userCamera);

    UpdateGates(connMgr, world, mousePos);

    if (connMgr->from) {
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) || IsKeyPressed(KEY_ESCAPE)) {
            connMgr->from = NULL;
            return;
        }
    }

    if (connMgr->from && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        TryConnectToNearestPort(connMgr, world, mousePos);
        connMgr->from = NULL;
    }
}

void DrawWirePreview(ConnMgr* connMgr, World* world) {
    if (!connMgr->from) return;

    Vector2 fromScreen = GetWorldToScreen2D(connMgr->from->position, world->userCamera);
    Vector2 toScreen   = GetMousePosition();

    DrawWireBezier(fromScreen, toScreen);
}

void AddWire(ConnMgr* connMgr, Wire wire) {
    if (connMgr->wireCount >= connMgr->wireCapacity) {
        connMgr->wireCapacity *= 2;
        connMgr->wires = realloc(connMgr->wires, sizeof(Wire) * connMgr->wireCapacity);
        if (!connMgr->wires) {
            connMgr->wireCapacity = 0;
            connMgr->wireCount = 0;
            return;
        }
    }
    connMgr->wires[connMgr->wireCount++] = wire;
}

void UnloadConnMgr(ConnMgr* connMgr) {
    free(connMgr->wires);
    connMgr->wires = NULL;
    connMgr->wireCount = 0;
    connMgr->wireCapacity = 0;
    connMgr->from = NULL;
}
