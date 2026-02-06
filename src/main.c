#include <raylib.h>
#include <stdbool.h>

#include "editor/editor.h"
#include "logic/gate.h"

int main(int argc, char* argv[]) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1920, 1080, "My Game");

    Editor editor = CreateEditor();

    AddGate(
        &editor.world, 
        CreateGate(NOT_GATE, (Vector2){ 200.0f, 200.0f})
    );

    AddGate(
        &editor.world, 
        CreateGate(AND_GATE, (Vector2){ 250.0f, 500.0f})
    );

    AddGate(
        &editor.world, 
        CreateGate(OR_GATE,  (Vector2){ 700.0f, 200.0f})
    );

    AddGate(
        &editor.world, 
        CreateGate(XOR_GATE, (Vector2){ 950.0f, 200.0f})
    );

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