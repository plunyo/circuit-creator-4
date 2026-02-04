#include "editor/editor.h"
#include "settings.h"
#include "world/world.h"
#include <raylib.h>

Editor CreateEditor() {
    Editor editor = { 0 };

    editor.world = CreateWorld();

    return editor;
}

void UpdateEditor(Editor* editor) {
    UpdateWorld(&editor->world);
}

void DrawEditor(Editor* editor) {
    DrawWorld(&editor->world);

    // editor bar
    int screenHeight = GetScreenHeight();
    DrawRectangle(0.0f, screenHeight * WORLD_HEIGHT_MULTIPLIER, GetScreenWidth(), screenHeight * WORLD_HEIGHT_MULTIPLIER, DARKGRAY);
}