#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <math.h>
#define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
 
// Two-channel sawtooth wave generator.
int saw( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData )
{
  unsigned int i; //, j;
  double *buffer = (double *) outputBuffer;
  //double *lastValues = (double *) userData;
 
  if ( status )
    std::cout << "Stream underflow detected!" << std::endl;
 
  // Write interleaved audio data.
  for ( i=0; i<nBufferFrames; i++ ) {
    //for ( j=0; j<1; j++ ) {
      //*buffer++ = lastValues[j];
 
      //lastValues[j] += 0.005 * (j+1+(j*0.1));
      buffer[i] = 16383.0 * sin(i*3000.0*2.0*M_PI/44100);
      //if ( lastValues[j] >= 1.0 ) lastValues[j] -= 2.0;
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
  std::cout << "\nPlaying ... \n";
  RtAudio::StreamParameters parameters;
  parameters.deviceId = dac.getDefaultOutputDevice();
  parameters.nChannels = 1; //2
  parameters.firstChannel = 0;
  unsigned int sampleRate = 44100;
  unsigned int bufferFrames = 44100; // 256 sample frames
  double data[1] = {0}; // data[2] = {0,0};-> buffer?
 
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
  //std::cout << "\nPlaying ... \n";
  //char input;
  SLEEP (100); // sleep 
  //std::cin.get( input );
  std::cout << "\nStopping ... \n";
  // Block released ... stop the stream
  //if ( dac.isStreamRunning() )
  //  dac.stopStream();  // or could call dac.abortStream();
  dac.abortStream();

 cleanup:
  if ( dac.isStreamOpen() ) dac.closeStream();
 
  return 0;
}