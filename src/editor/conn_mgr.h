#pragma once

#include "world/world.h"

typedef struct ConnMgr {
    void* from;
} ConnMgr;

void UpdateConnector(ConnMgr* connMgr, World* world);