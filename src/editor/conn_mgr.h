#pragma once

#include <stddef.h>
#include "logic/wire.h"

typedef struct World World;

typedef struct ConnMgr {
    Port* from;

    Wire*  wires;
    size_t wireCapacity;
    int    wireCount;
} ConnMgr;

ConnMgr CreateConnMgr();
void UpdateConnMgr(ConnMgr* connMgr, World* world);
void AddWire(ConnMgr *connMgr, Wire wire);
void UnloadConnMgr(ConnMgr* connMgr);
void DrawWirePreview(ConnMgr* connMgr, World* world);