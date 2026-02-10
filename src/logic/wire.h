#pragma once

#include "logic/port.h"

typedef struct Wire {
    Port* from;
    Port* to;
} Wire;

Wire CreateWire(void* from, void* to);
void DrawWire(Wire* wire);