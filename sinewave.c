// taken from https://batchloaf.wordpress.com/2017/02/10/a-simple-way-to-read-and-write-audio-and-video-files-in-c-using-ffmpeg/

// To compile: gcc sinewave.c -o sinewave -lm

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define N 44100                 // number of samples

void main()
{
    // create audio buffer
    int16_t buf[N] = {0};       // buffer (16-bit signed integer samples)
    int n;                      // buffer index
    double Fs = 44100.0;        // sampling frequency

    // Generate 1 second of audio data
    for (n=0 ; n<N ; ++n) buf[n] = 16383.0 * sin(n*3000.0*2.0*M_PI/Fs);

    // Pipe to ffmpeg for wav file
    FILE *pipeout;
    pipeout = popen("C:/Users/Nolan/ffmpeg/bin/ffmpeg.exe -y -f s16le -ar 44100 -ac 1 -i - sine.wav", "w");
    fwrite(buf, 2, N, pipeout);
    pclose(pipeout);
}