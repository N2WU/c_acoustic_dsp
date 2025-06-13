#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <math.h>
#include <vector>
#include <complex>
#include <bits/stdc++.h>
#define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
using namespace std::complex_literals;
// Single-Channel Sine Wave generator.
int bpsk( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData )
{
  //unsigned int n;
  int data_len = 1000;
  double Fs = 48000;
  unsigned int R = 3000;
  double fc = 5000;
  unsigned int NsPs = Fs/R;
  std::vector<int> rand_bits(data_len);
  //vector<complex<double>> ref = {1,-1}
  double *buffer = (double *) outputBuffer;

  for (int i = 0; i < data_len; i++){ //Generate BPSk Symbols
   rand_bits[i] = rand() % 1;
   rand_bits[i] *= 2;
   rand_bits[i] += -1;
  }
  //resize operation for symbols would happen here

  //upsample data symbols by NsPs
  int n_bits = NsPs * data_len;
  std::vector<double> up_bits(n_bits);
  for (int i = 0; i < data_len; i++){
    for (unsigned int ns = 0; ns < NsPs; ns++){
      up_bits[i*NsPs+ns] = rand_bits[i];
    }
  }

  if ( status )
    std::cout << "Stream underflow detected!" << std::endl;
 
  // Write audio data; n is the same rate as i
  //for ( n=0; n<nBufferFrames; n++ ) { //for the whole buffer (can have zeros on the end)
    for (int nn=0; nn<n_bits; nn++){ // for the upshifted 
      // gives you a little period
      double nnd = nn;
      std::complex<double> comp_bit;
      std::complex<double> comp_exp = (2i * fc * M_PI * nnd/ Fs);
      comp_bit = up_bits[nn] * std::exp(comp_exp);
    
    if (nn < nBufferFrames){
    buffer[nn] = real(comp_bit);
    }
    }
  //}
 
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
  unsigned int sampleRate = 48000;
  unsigned int bufferFrames = 48000; // 256 sample frames
  double data[1] = {0};
 
  if ( dac.openStream( &parameters, NULL, RTAUDIO_FLOAT64, sampleRate,
                       &bufferFrames, &bpsk, (void *)&data ) ) {
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