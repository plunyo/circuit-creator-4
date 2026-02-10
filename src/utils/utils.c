#include "utils/utils.h"

#include <math.h>
#include "settings.h"

int RectangleContainsRect(Rectangle a, Rectangle b) {
    return b.x >= a.x && b.x + b.width <= a.x + a.width &&
           b.y >= a.y && b.y + b.height <= a.y + a.height;
}

void DrawWireBezier(Vector2 p1, Vector2 p4) {
    float dx = p4.x - p1.x;

    float bend = fmaxf(fabsf(dx) * 0.4f, 50.0f);
    if (dx < 0) bend = -bend;

    Vector2 c2 = (Vector2){ p1.x + bend, p1.y };
    Vector2 c3 = (Vector2){ p4.x - bend, p4.y };

    DrawSplineSegmentBezierCubic(p1, c2, c3, p4, PORT_RADIUS * 0.8f, DARKGRAY);
}
