#pragma once

#include <raylib.h>

typedef enum GateType {
    NOT_GATE,
    XOR_GATE,
    OR_GATE,
    AND_GATE,
} GateType;

typedef struct Gate {
    GateType type;

    Vector2 position;
    
    bool inputs[2]; // gates will only have 2 outputs at one time
    bool output;
} Gate;

Gate CreateGate(GateType type, Vector2 pos);
void UpdateGate(Gate* gate);
void DrawGate(Gate* gate);