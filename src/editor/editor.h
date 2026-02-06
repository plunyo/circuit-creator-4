#pragma once

#include "editor/connector.h"
#include "world/world.h"

typedef struct Editor {
    World world;
    Connector* connManager;
} Editor;

Editor CreateEditor();
void UpdateEditor(Editor* editor);
void DrawEditor(Editor* editor);