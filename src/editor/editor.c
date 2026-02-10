#include "editor/editor.h"
#include "editor/conn_mgr.h"
#include "stdlib.h"
#include "settings.h"
#include <raylib.h>

Editor CreateEditor() {
    Editor editor = { 0 };

    editor.connMgr = malloc(sizeof(ConnMgr));
    *editor.connMgr = CreateConnMgr();

    return editor;
}

void UpdateEditor(Editor* editor, World* world) {
    UpdateConnMgr(editor->connMgr, world);
}

void DrawEditor(Editor* editor, World* world) {
    // wire preview
    DrawWirePreview(editor->connMgr, world);

    // editor bar
    int screenHeight = GetScreenHeight();

    const Rectangle editorBarRect = (Rectangle){
        EDITOR_BAR_MARGIN, 
        screenHeight - EDITOR_BAR_MARGIN - EDITOR_BAR_HEIGHT, 
        GetScreenWidth() - EDITOR_BAR_MARGIN * 2, 
        EDITOR_BAR_HEIGHT
    };

    DrawRectangleRec(editorBarRect, DARKGRAY);
    DrawRectangleLinesEx(editorBarRect, EDITOR_BAR_OUTLINE, LIGHTGRAY);
}