#include <raylib.h>
#include <stdbool.h>

#include "editor/editor.h"

int main(int argc, char* argv[]) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1920, 1080, "My Game");

    Editor editor = CreateEditor();

    AddGate(&editor.world, (Gate){ AND_GATE, (Vector2){100, 100}, { false, false}, { false } });
    AddGate(&editor.world, (Gate){ OR_GATE,  (Vector2){300, 100}, { false, false}, { false } });
    AddGate(&editor.world, (Gate){ XOR_GATE, (Vector2){500, 100}, { false, false}, { false } });
    AddGate(&editor.world, (Gate){ NOT_GATE, (Vector2){700, 100}, { false, false}, { false } });
    

    while (!WindowShouldClose()) {
        UpdateEditor(&editor);

        BeginDrawing();
            ClearBackground(BLACK);

            DrawEditor(&editor);

            DrawText(TextFormat("FPS: %d", GetFPS()), 15, 15, 30, BLACK);
        EndDrawing();
    }

    UnloadWorld(&editor.world);
    CloseWindow();
    return 0;
}