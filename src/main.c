#include <raylib.h>
#include <stdlib.h>
#include <time.h>

#include "editor/editor.h"
#include "world/world.h"
#include "logic/gate.h"

int main(int argc, char* argv[]) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1920, 1080, "circuit creator 4");

    srand((unsigned int)time(NULL));

    World world = CreateWorld();
    Editor editor = CreateEditor();

    int numRandomGates = 100;
    for (int i = 0; i < numRandomGates; i++) {
        GateType type = rand() % 4;
        Vector2 pos = {
            (float)(rand() % 10000), // x 0-4000
            (float)(rand() % 10000)
        };
        AddGate(&world, CreateGate(type, pos));
    }

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        UpdateWorld(&world, deltaTime);
        UpdateEditor(&editor, &world);

        if (IsKeyPressed(KEY_F11)) MaximizeWindow();
        

        BeginDrawing();
            ClearBackground(GRAY);

            DrawWorld(&world, editor.connMgr);
            DrawEditor(&editor, &world);

            DrawText(TextFormat("FPS: %d", GetFPS()), 15, 15, 30, BLACK);
        EndDrawing();
    }

    UnloadWorld(&world);
    CloseWindow();
    return 0;
}
