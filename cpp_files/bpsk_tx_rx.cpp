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


// This should generate user-specified BPSK bits and output a pointer to
// s, a real discrete signal for audio out
int bpsk(int data_len, double Fs, unsigned int R, double fc, void *inputData, 
   void *s)
{
  unsigned int NsPs = Fs/R;


  // Generate random bits
  std::vector<int> rand_bits(data_len);
  double *fake_s = (double *) s;
  // handle inputData

  // Generate BPSK Symbols
  for (int i = 0; i < data_len; i++){ 
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

  // Write audio data; n is the same rate as i
  for (int nn=0; nn<n_bits; nn++){ // for the upshifted 
    // gives you a little period
    double nnd = nn;
    std::complex<double> comp_up_bit = up_bits[nn];
    std::complex<double> comp_bit;
    std::complex<double> comp_exp = (2i * fc * M_PI * nnd/ Fs);
    double s_bit;
    comp_bit = comp_up_bit * std::exp(comp_exp);
    s_bit = std::real(comp_bit);
    *fake_s++ = s_bit;
    }
  std::cout << "\nBPSK Done\n";
  return 0;
}

// This should be an easy input-output where it just takes userData
//  and moves them to the outputBuffer
int gen_wave( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData ) //void
{
  double *buffer = (double *) outputBuffer;
  double *bytes = (double *) userData;

  std::cout << nBufferFrames << std::endl;
  if ( status ) {std::cout << "Stream underflow detected!" << std::endl;}
  //double *bytes = (double *) userData;
  //memcpy( outputBuffer, inputBuffer, *bytes );
  for (int nn=0; nn<nBufferFrames; nn++){ 
    if (nn < nBufferFrames){
    std::cout << nn << std::endl;
    buffer[nn] = bytes[nn];
    }
  }
  // I would probably put channel assignments here...

  return 0;
}
 
int main()
{
  // Start RtAudio
  RtAudio dac;
  std::vector<unsigned int> deviceIds = dac.getDeviceIds();
  if ( deviceIds.size() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
    exit( 0 );
  }
  std::cout << "\nPlaying ... \n";
  RtAudio::StreamParameters parameters;
  parameters.deviceId = dac.getDefaultOutputDevice();
  parameters.nChannels = 1;
  parameters.firstChannel = 0;
  unsigned int sampleRate = 48000;
  unsigned int bufferFrames = 16000; //uhhhh

  // Specify BPSK 
  int data_len = 1000;
  double Fs = sampleRate;
  unsigned int R = 3000;
  double fc = 5000;
  double * inputData;
  double s[16000] = {0};
  
  if ( bpsk(data_len, Fs, R, fc, (void *)inputData, (void *)&s)){ //&s
    std::cout << '\n' << "Issue with generating BPSK symbols" << '\n' << std::endl;
  }
  if ( dac.openStream( &parameters, NULL, RTAUDIO_FLOAT64, sampleRate,
                       &bufferFrames, &gen_wave, &s ) ) {
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