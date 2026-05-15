#ifndef STATE_H
#define STATE_H

struct BPSKState {
    int bitIndex;
    int symbolCounter;
    double phase;
    int currentBit;
};

#endif