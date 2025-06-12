#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <math.h>
#define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
 
// Single-Channel Sine Wave generator.
int saw( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData )
{
  unsigned int i;
  double *buffer = (double *) outputBuffer;
 
  if ( status )
    std::cout << "Stream underflow detected!" << std::endl;
 
  // Write interleaved audio data.
  for ( i=0; i<nBufferFrames; i++ ) {
    buffer[i] = 16383.0 * sin(i*3000.0*2.0*M_PI/44100);
  }
 
  return 0;
}
 
int main()
{
  RtAudio dac;
  std::vector<unsigned int> deviceIds = dac.getDeviceIds();
  if ( deviceIds.size() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
    exit( 0 );
  }
  std::cout << "\nPlaying ... \n";
  RtAudio::StreamParameters parameters;
  parameters.deviceId = dac.getDefaultOutputDevice();
  parameters.nChannels = 1; //2
  parameters.firstChannel = 0;
  unsigned int sampleRate = 44100;
  unsigned int bufferFrames = 44100; // 256 sample frames
  double data[1] = {0};
 
  if ( dac.openStream( &parameters, NULL, RTAUDIO_FLOAT64, sampleRate,
                       &bufferFrames, &saw, (void *)&data ) ) {
    std::cout << '\n' << dac.getErrorText() << '\n' << std::endl;
    exit( 0 ); // problem with device settings
  }
 
  // Stream is open ... now start it.
  if ( dac.startStream() ) {
    std::cout << dac.getErrorText() << std::endl;
    goto cleanup;
  }
  SLEEP (100); // sleep 
  std::cout << "\nStopping ... \n";
  // Block released ... stop the stream
  dac.abortStream();

 cleanup:
  if ( dac.isStreamOpen() ) dac.closeStream();
 
  return 0;
}