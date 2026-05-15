#include "bpsk.h"

#include <iostream>
#include <cmath>

int bpsk(void *outputBuffer,
         void *inputBuffer,
         unsigned int nBufferFrames,
         double streamTime,
         RtAudioStreamStatus status,
         void *userData)
{
    if (status)
        std::cout << "Stream over/underflow detected.\n";

    double *in  = static_cast<double *>(inputBuffer);
    double *out = static_cast<double *>(outputBuffer);

    BPSKState *st = static_cast<BPSKState *>(userData);

    // carrier increment
    const double phaseInc =
        2.0 * M_PI * st->fc / st->Fs;

    for (unsigned int n = 0; n < nBufferFrames; n++)
    {
        // ---------------------------------
        // Integrate microphone samples
        // ---------------------------------
        st->accumulator += in[n];

        // ---------------------------------
        // Symbol clock
        // ---------------------------------
        if (st->symbolCounter == 0)
        {
            // symbol decision
            st->currentBit =
                (st->accumulator >= 0.0) ? 1 : -1;

            // reset integrator
            st->accumulator = 0.0;
        }

        st->symbolCounter++;

        if (st->symbolCounter >= st->NsPs)
            st->symbolCounter = 0;

        // ---------------------------------
        // BPSK carrier generation
        // ---------------------------------

        // BPSK:
        // bit = +1 -> phase 0
        // bit = -1 -> phase pi

        double sample =
            st->currentBit * std::cos(st->phase);

        out[n] = sample;

        // ---------------------------------
        // Continuous phase oscillator
        // ---------------------------------
        st->phase += phaseInc;

        if (st->phase >= 2.0 * M_PI)
            st->phase -= 2.0 * M_PI;
    }

    return 0;
}