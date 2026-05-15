#ifndef BPSK_H
#define BPSK_H

#include "RtAudio.h"

struct BPSKState
{
    // carrier state
    double phase;

    // symbol timing
    unsigned int symbolCounter;
    int currentBit;

    // mic integration
    double accumulator;

    // parameters
    double Fs;
    double R;
    double fc;
    unsigned int NsPs;
};

int bpsk(void *outputBuffer,
         void *inputBuffer,
         unsigned int nBufferFrames,
         double streamTime,
         RtAudioStreamStatus status,
         void *userData);

#endif