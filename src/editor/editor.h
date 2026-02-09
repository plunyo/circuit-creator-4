#pragma once

typedef struct ConnMgr ConnMgr;

typedef struct Editor {
    ConnMgr* connManager;
} Editor;

Editor CreateEditor();
void UpdateEditor(Editor* editor);
void DrawEditor(Editor* editor);