#include <raylib.h>
#include <stdlib.h>
#include <time.h>

#include "editor/editor.h"
#include "world/world.h"
#include "logic/gate.h"

int main(int argc, char* argv[]) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1920, 1080, "Gate Madness");

    srand((unsigned int)time(NULL)); // random seed

    World world = CreateWorld();
    Editor editor = CreateEditor();

    // spawn 50 random gates all over the place
    int numRandomGates = 1000;
    for (int i = 0; i < numRandomGates; i++) {
        GateType type = rand() % 4; // 0=NOT,1=AND,2=OR,3=XOR,4=NAND
        Vector2 pos = {
            (float)(rand() % 10000), // x 0-4000
            (float)(rand() % 10000)  // y 0-4000
        };
        AddGate(&world, CreateGate(type, pos));
    }

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        UpdateWorld(&world, dt);
        UpdateEditor(&editor);

        BeginDrawing();
            ClearBackground(GRAY);

            DrawWorld(&world);
            DrawEditor(&editor);

            DrawText(TextFormat("FPS: %d", GetFPS()), 15, 15, 30, BLACK);
        EndDrawing();
    }

    UnloadWorld(&world);
    CloseWindow();
    return 0;
}
