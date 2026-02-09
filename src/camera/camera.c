#include "camera/camera.h"

#include <math.h>
#include <raylib.h>
#include <raymath.h>

Camera2D CreateUserCamera(Vector2 target) {
    Camera2D userCamera;

    userCamera.target = target;
    userCamera.offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    userCamera.rotation = 0.0f;
    userCamera.zoom = 1.0f;

    return userCamera;
}

void UpdateUserCamera(Camera2D* userCamera, float deltaTime) {
    userCamera->zoom = expf(
        logf(userCamera->zoom) + ((float)GetMouseWheelMove() * 0.1f)
    );

    if (userCamera->zoom > MAX_ZOOM) userCamera->zoom = MAX_ZOOM;
    else if (userCamera->zoom < MIN_ZOOM) userCamera->zoom = MIN_ZOOM;

    float speed = IsKeyDown(KEY_LEFT_SHIFT) ? CAMERA_SPEED * SPRINT_MULTIPLIER : CAMERA_SPEED;
    speed /= userCamera->zoom;

    if (IsKeyDown(KEY_W)) {
        userCamera->target.y -= speed * deltaTime;
    }
    if (IsKeyDown(KEY_S)) {
        userCamera->target.y += speed * deltaTime;
    }
    if (IsKeyDown(KEY_A)) {
        userCamera->target.x -= speed * deltaTime;
    }
    if (IsKeyDown(KEY_D)) {
        userCamera->target.x += speed * deltaTime;
    }
}

void BeginUserCameraMode(Camera2D userCamera) {
    BeginMode2D(userCamera);
}

void EndUserCameraMode() {
    EndMode2D();
}

Rectangle GetUserCameraRect(Camera2D userCamera) {
    Vector2 topLeft = GetScreenToWorld2D((Vector2){ 0, 0 }, userCamera);
    Vector2 bottomRight = GetScreenToWorld2D((Vector2){ GetScreenWidth(), GetScreenHeight() }, userCamera);

    float width = bottomRight.x - topLeft.x;
    float height = bottomRight.y - topLeft.y;

    return (Rectangle){ topLeft.x, topLeft.y, width, height };
}