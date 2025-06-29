#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
 
// Pass-through function.
int inout( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *data )
{
  // Since the number of input and output channels is equal, we can do
  // a simple buffer copy operation here.
  if ( status ) std::cout << "Stream over/underflow detected." << std::endl;
 
  unsigned int *bytes = (unsigned int *) data;
  memcpy( outputBuffer, inputBuffer, *bytes );
  return 0;
}
 
int main()
{
  RtAudio adac;
  std::vector<unsigned int> deviceIds = adac.getDeviceIds();
  if ( deviceIds.size() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
    exit( 0 );
  }
 
  // Set the same number of channels for both input and output.
  unsigned int bufferBytes, bufferFrames = 512;
  RtAudio::StreamParameters iParams, oParams;
  iParams.deviceId = deviceIds[0]; // first available device
  iParams.nChannels = 1;
  oParams.deviceId = deviceIds[2]; // first available device
  oParams.nChannels = 1;
 
  if ( adac.openStream( &oParams, &iParams, RTAUDIO_SINT32, 48000,
                        &bufferFrames, &inout, (void *)&bufferBytes ) ) {
    std::cout << '\n' << adac.getErrorText() << '\n' << std::endl;
    exit( 0 ); // problem with device settings
  }
 
  bufferBytes = bufferFrames * 2 * 4;
 
  if ( adac.startStream() ) {
    std::cout << adac.getErrorText() << std::endl;
    goto cleanup;
  }
 
  char input;
  std::cout << "\nRunning ... press <enter> to quit.\n";
  std::cin.get(input);
 
  // Block released ... stop the stream
  if ( adac.isStreamRunning() )
    adac.stopStream();  // or could call adac.abortStream();
 
 cleanup:
  if ( adac.isStreamOpen() ) adac.closeStream();
 
  return 0;
}