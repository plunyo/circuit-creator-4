#include "utils/utils.h"

int RectangleContainsRect(Rectangle a, Rectangle b) {
    return b.x >= a.x && b.x + b.width <= a.x + a.width &&
           b.y >= a.y && b.y + b.height <= a.y + a.height;
}