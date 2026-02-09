#pragma once

#include "editor/conn_mgr.h"
#include "world/world.h"

typedef struct Editor {
    World world;
    ConnMgr* connManager;
} Editor;

Editor CreateEditor();
void UpdateEditor(Editor* editor);
void DrawEditor(Editor* editor);