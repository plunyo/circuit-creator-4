#pragma once

#include "editor/conn_mgr.h"

typedef struct Editor {
    ConnMgr* connManager;
} Editor;

Editor CreateEditor();
void UpdateEditor(Editor* editor);
void DrawEditor(Editor* editor);