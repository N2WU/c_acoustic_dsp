#define _USE_MATH_DEFINES

#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
 
// Single-channel sine wave generator.
int sine( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData )
{
  unsigned int i, j;
  double *buffer = (double *) outputBuffer;
  double *lastValues = (double *) userData;
 
  if ( status )
    std::cout << "Stream underflow detected!" << std::endl;
 
  // Write interleaved audio data.
  for ( i=0; i<nBufferFrames; i++ ) {
    //for ( j=0; j<2; j++ ) {
    // buf[n] = 16383.0 * sin(n*3000.0*2.0*M_PI/Fs);
      
 
      lastValues[i] += 16383.0 * sin(n*3000.0*2.0*M_PI/44100);
      *buffer++ = lastValues[i];
    //  if ( lastValues[j] >= 1.0 ) lastValues[j] -= 2.0;
    //}
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
 
  RtAudio::StreamParameters parameters;
  parameters.deviceId = dac.getDefaultOutputDevice();
  parameters.nChannels = 2;
  parameters.firstChannel = 0;
  unsigned int sampleRate = 44100;
  unsigned int bufferFrames = 256; // 256 sample frames
  double data[2] = {0, 0};
 
  if ( dac.openStream( &parameters, NULL, RTAUDIO_FLOAT64, sampleRate,
                       &bufferFrames, &sine, (void *)&data ) ) {
    std::cout << '\n' << dac.getErrorText() << '\n' << std::endl;
    exit( 0 ); // problem with device settings
  }
 
  // Stream is open ... now start it.
  if ( dac.startStream() ) {
    std::cout << dac.getErrorText() << std::endl;
    goto cleanup;
  }
  
  char input;
  std::cout << "\nPlaying ... press <enter> to quit.\n";
  std::cin.get( input );
 
  // Block released ... stop the stream
  if ( dac.isStreamRunning() )
    dac.stopStream();  // or could call dac.abortStream();
 
 cleanup:
  if ( dac.isStreamOpen() ) dac.closeStream();
 
  return 0;