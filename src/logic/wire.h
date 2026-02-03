#pragma once

typedef struct Wire {
    void* from; // either gate* or chip*
    void* to;   // either gate* or chip*

    int fromOutputIndex; // which output
    int toInputIndex;    // which input
} Wire;