#pragma once

#include <raylib.h>

#define ZOOM_SPEED 0.1f
#define CAMERA_SPEED 800.0f
#define SPRINT_MULTIPLIER 1.8f

#define MIN_ZOOM 0.25f
#define MAX_ZOOM 5.0f

#define ZOOM_LERP_SPEED 8.0f
#define MOVE_LERP_SPEED 10.0f

Camera2D CreateUserCamera(Vector2 target);
void UpdateUserCamera(Camera2D* camera, float deltaTime);
void BeginUserCameraMode(Camera2D camera);
void EndUserCameraMode();

Rectangle GetUserCameraRect(Camera2D camera);