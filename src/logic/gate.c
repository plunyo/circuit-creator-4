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
    gate.output = (Port){ 0 };
    
    MoveGate(&gate, pos);   

    return gate;
}

void UpdateGate(Gate* gate) {
    switch (gate->type) {
        case AND_GATE:
            gate->output.value= gate->inputs[0].value && gate->inputs[1].value;
            break;

        case OR_GATE:
            gate->output.value = gate->inputs[0].value || gate->inputs[1].value;
            break;

        case XOR_GATE:
            gate->output.value = gate->inputs[0].value != gate->inputs[1].value;
            break;

        case NOT_GATE:
            gate->output.value = !gate->inputs[0].value; // only first input matters
            break;
    }
}

void MoveGate(Gate* gate, Vector2 newPos) {
    static const float halfHeight = GATE_HEIGHT * 0.5f;
    static const float quarterHeight = GATE_HEIGHT * 0.25f;

    gate->position = newPos;

    if (gate->type != NOT_GATE) {
        gate->inputs[0].position = (Vector2){ newPos.x, newPos.y + quarterHeight };
        gate->inputs[1].position = (Vector2){ newPos.x, newPos.y + halfHeight + quarterHeight };
    } else {
        gate->inputs[0].position = (Vector2){ newPos.x, newPos.y + halfHeight };
    }

    gate->output.position = (Vector2){newPos.x + GATE_WIDTH, newPos.y + halfHeight };
}
 
void DrawGate(Gate *gate) {
    const float x = gate->position.x;
    const float y = gate->position.y;

    const Rectangle gateRect = { x, y, GATE_WIDTH, GATE_HEIGHT };

    // main body
    DrawRectanglePro(gateRect, (Vector2){0, 0}, 0.0f, getColorFromGate(gate->type));
    DrawRectangleLinesEx(gateRect, 5.0f, ColorBrightness(getColorFromGate(gate->type), -0.1f));

    // input ports
    DrawCircleV(gate->inputs[0].position, GATE_PORT_RADIUS, GATE_PORT_COLOR);
    
    if (gate->type != NOT_GATE) {
        DrawCircleV(gate->inputs[1].position, GATE_PORT_RADIUS, GATE_PORT_COLOR);
    }

    // output port
    DrawCircleV(gate->output.position, GATE_PORT_RADIUS, GATE_PORT_COLOR);

    // text
    const char* text = getNameFromGate(gate->type);
    int textWidth = MeasureText(text, GATE_FONT_SIZE);

    float textX = x + (GATE_WIDTH  - textWidth) / 2.0f;
    float textY = y + (GATE_HEIGHT - GATE_FONT_SIZE) / 2.0f;

    DrawText(text, textX, textY, GATE_FONT_SIZE, RAYWHITE);

}