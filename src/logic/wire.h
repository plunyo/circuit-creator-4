#pragma once

#include "logic/port.h"

typedef struct Wire {
    Port* from;
    Port* to;
} Wire;