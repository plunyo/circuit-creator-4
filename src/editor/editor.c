#include "editor/editor.h"
#include "settings.h"
#include <raylib.h>

Editor CreateEditor() {
    Editor editor = { 0 };

    editor.connManager = MemAlloc(sizeof(ConnMgr));

    return editor;
}

void UpdateEditor(Editor* editor) {
}

void DrawEditor(Editor* editor) {
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