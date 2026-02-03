#include <raylib.h>
#include <stdbool.h>

#include "world/world.h"

int main(int argc, char* argv[]) {
    InitWindow(1920, 1080, "My Game");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    World world = CreateWorld();

    AddGate(&world, (Gate){AND_GATE, (Vector2){100, 100}, { false, false}, true});
    AddGate(&world, (Gate){OR_GATE, (Vector2){300, 100}, { false, false}, true});
    AddGate(&world, (Gate){XOR_GATE, (Vector2){500, 100}, { false, false}, true});
    AddGate(&world, (Gate){NOT_GATE, (Vector2){700, 100}, { false, false}, true});
    

    while (!WindowShouldClose()) {
        UpdateWorld(&world);

        BeginDrawing();
            ClearBackground(BLACK);

            DrawWorld(&world);

            DrawText(TextFormat("FPS: %d", GetFPS()), 15, 15, 30, BLACK);
        EndDrawing();
    }

    UnloadWorld(&world);
    CloseWindow();
    return 0;
}