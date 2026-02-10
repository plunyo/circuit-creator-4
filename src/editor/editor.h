#pragma once

#include "world/world.h"

typedef struct ConnMgr ConnMgr;

typedef struct Editor {
    ConnMgr* connMgr;
} Editor;

Editor CreateEditor();
void UpdateEditor(Editor* editor, World* world);
void DrawEditor(Editor* editor, World* world);