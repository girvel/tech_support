#include <raylib.h>

int main()
{
    InitWindow(400, 400, "Tech Support");
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Hello, world!", 100, 100, 12, RAYWHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
