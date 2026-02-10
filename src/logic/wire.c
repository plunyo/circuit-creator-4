#include "logic/wire.h"

#include <raylib.h>
#include "utils/utils.h"

Wire CreateWire(void* from, void* to) {
    Wire wire = { 0 };

    wire.from = from;
    wire.to   = to;

    return wire;
}

void DrawWire(Wire* wire) {
    Vector2 p1 = wire->from->position;
    Vector2 p4 = wire->to->position;

    DrawWireBezier(p1, p4);
}