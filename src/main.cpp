#include "RtAudio.h"
#include "bpsk.h"

#include <iostream>
#include <vector>
#include <unistd.h>

#define SLEEP(ms) usleep((unsigned long)(ms * 1000.0))

int main()
{
    RtAudio adac;

    if (adac.getDeviceIds().size() < 1)
    {
        std::cout << "No audio devices found!\n";
        return 0;
    }

    // ---------------------------------
    // INPUT DEVICE
    // ---------------------------------
    RtAudio::StreamParameters iParams;
    iParams.deviceId = adac.getDefaultInputDevice();
    iParams.nChannels = 1;
    iParams.firstChannel = 0;

    // ---------------------------------
    // OUTPUT DEVICE
    // ---------------------------------
    RtAudio::StreamParameters oParams;
    oParams.deviceId = adac.getDefaultOutputDevice();
    oParams.nChannels = 1;
    oParams.firstChannel = 0;

    // ---------------------------------
    // DSP STATE
    // ---------------------------------
    BPSKState state;

    state.phase = 0.0;

    state.symbolCounter = 0;
    state.currentBit = 1;

    state.accumulator = 0.0;

    state.Fs = 48000.0;
    state.R  = 3000.0;
    state.fc = 5000.0;

    state.NsPs =
        static_cast<unsigned int>(state.Fs / state.R);

    // ---------------------------------
    // STREAM CONFIG
    // ---------------------------------
    unsigned int sampleRate = 48000;
    unsigned int bufferFrames = 512;

    // FULL DUPLEX STREAM
    if (adac.openStream(
            &oParams,
            &iParams,
            RTAUDIO_FLOAT64,
            sampleRate,
            &bufferFrames,
            &bpsk,
            (void *)&state))
    {
        std::cout << adac.getErrorText() << std::endl;
        return 0;
    }

    // ---------------------------------
    // START STREAM
    // ---------------------------------
    if (adac.startStream())
    {
        std::cout << adac.getErrorText() << std::endl;
        goto cleanup;
    }

    std::cout << "\nRunning BPSK TX...\n";
    std::cout << "Press ENTER to quit.\n";

    std::cin.get();

    if (adac.isStreamRunning())
        adac.stopStream();

cleanup:

    if (adac.isStreamOpen())
        adac.closeStream();

    return 0;
}