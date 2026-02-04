#pragma once

#include "editor/connections.h"
#include "world/world.h"

typedef struct Editor {
    World world;
    ConnectionManager* connManager;
} Editor;

Editor CreateEditor();
void UpdateEditor(Editor* editor);
void DrawEditor(Editor* editor);