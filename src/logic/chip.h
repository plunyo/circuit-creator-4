#pragma once

#include <raylib.h>
#include "logic/gate.h"
#include "logic/wire.h"

typedef struct Chip {
    Vector2 pos;

    Gate* gates;            // internal gates
    int gateCount;

    Wire* wires;            // internal wires
    int wireCount;

    int inputCount;         // number of external input pins
    int outputCount;        // number of external output pins
    
    bool* inputs;           // array of external input values
    bool* outputs;          // array of external output values
} Chip;
