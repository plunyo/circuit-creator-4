#pragma once

#include <raylib.h>
#include "port.h"

typedef enum GateType {
    NOT_GATE,
    XOR_GATE,
    OR_GATE,
    AND_GATE,
} GateType;

typedef struct Gate {
    GateType type;

    Vector2 position;
    
    Port inputs[2];
    Port output;
} Gate;

Gate CreateGate(GateType type, Vector2 pos);
void UpdateGate(Gate* gate);
void MoveGate(Gate* gate, Vector2 newPos);
void DrawGate(Gate* gate);