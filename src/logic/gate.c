#include "gate.h"

#include <raylib.h>
#include "settings.h"

static char* getNameFromGate(GateType type) {
    switch (type) {
        case NOT_GATE: return "NOT";
        case OR_GATE:  return "OR";
        case XOR_GATE: return "XOR";
        case AND_GATE: return "AND";
    }

    return "NIL";
}

static Color getColorFromGate(GateType type) {
    switch (type) {
        case NOT_GATE: return RED;
        case OR_GATE:  return GREEN;
        case XOR_GATE: return PURPLE;
        case AND_GATE: return BLUE;
    }

    return WHITE;
}

Gate CreateGate(GateType type, Vector2 pos) {
    Gate gate = { 0 };

    gate.type = type;
    gate.position = pos;

    return gate;
}

void UpdateGate(Gate* gate) {
    switch (gate->type) {
        case AND_GATE:
            gate->output[0] = gate->inputs[0] && gate->inputs[1];
            break;

        case OR_GATE:
            gate->output[0] = gate->inputs[0] || gate->inputs[1];
            break;

        case XOR_GATE:
            gate->output[0] = gate->inputs[0] != gate->inputs[1];
            break;

        case NOT_GATE:
            gate->output[0] = !gate->inputs[0]; // only first input matters
            break;
    }
}

 
void DrawGate(Gate *gate) {
    const float x = gate->position.x;
    const float y = gate->position.y;
    const float halfHeight = GATE_HEIGHT * 0.5f;
    const float quarterHeight = GATE_HEIGHT * 0.25f;

    const Rectangle gateRect = { x, y, GATE_WIDTH, GATE_HEIGHT };

    
    // main body
    DrawRectanglePro(gateRect, (Vector2){0, 0}, 0.0f, getColorFromGate(gate->type));
    DrawRectangleLinesEx(gateRect, 5.0f, ColorAlpha(getColorFromGate(gate->type), 0.4f));

    // input ports
    if (gate->type != NOT_GATE) {
        DrawCircleV((Vector2){ x, y + quarterHeight }, GATE_PORT_RADIUS, GATE_PORT_COLOR);
        DrawCircleV((Vector2){ x, y + GATE_HEIGHT - quarterHeight}, GATE_PORT_RADIUS, GATE_PORT_COLOR);
    } else {
        // because not gates have only one output
        DrawCircleV((Vector2){ x, y + halfHeight }, GATE_PORT_RADIUS, GATE_PORT_COLOR);
    }

    // output port
    DrawCircleV((Vector2){ x + GATE_WIDTH, y + halfHeight }, GATE_PORT_RADIUS, GATE_PORT_COLOR);

    // text
    const char* text = getNameFromGate(gate->type);
    int textWidth = MeasureText(text, GATE_FONT_SIZE);

    float textX = gate->position.x + (GATE_WIDTH  - textWidth) / 2.0f;
    float textY = gate->position.y + (GATE_HEIGHT - GATE_FONT_SIZE) / 2.0f;

    DrawText(text, textX, textY, GATE_FONT_SIZE, RAYWHITE);

}