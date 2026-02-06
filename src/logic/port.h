#pragma once

#include <raylib.h>

typedef enum PortType {
    PORT_INPUT,
    PORT_OUTPUT
} PortType;

typedef struct Port {
    Vector2 position;
    PortType type;
    bool value;
} Port;