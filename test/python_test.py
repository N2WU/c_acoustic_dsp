import numpy as np 

data_len = 100

rand_bits = 2*np.random.randint(0,2,data_len) - 1

# resample

Fs = 48000
R = 3000
fc = 5000
NsPs = int(Fs/R)
n_bits = int(data_len*NsPs)
up_bits = np.zeros(n_bits,)

for i in range(data_len):
    #for (int i = 0; i < data_len; i++){
    #   for (unsigned int ns = 0; ns < NsPs; ns++){
    #       up_bits[i*NsPs+ns] = rand_bits[i];
    #   }
    #}
    for k in range(NsPs):
        up_bits[i*NsPs+k] = rand_bits[i]


#for (int nn=0; nn<n_bits; nn++){ // for the upshifted 
#      // gives you a little period
#      double nnd = nn;
#      std::complex<double> comp_bit;
#      std::complex<double> comp_exp = (2i * fc * M_PI * nnd/ Fs);
#      comp_bit = up_bits[nn] * std::exp(comp_exp);
#    
#    if (nn < nBufferFrames){
#    buffer[nn] = real(comp_bit);
#       }
#    }

s = np.zeros((n_bits,),dtype=complex)

for ii in range(n_bits):
    expo = np.exp(2j*np.pi*fc*ii/Fs)
    s[ii] = up_bits[ii]*expo #[ii]

# try again with matrix operations
s2 = up_bits * np.exp(2j*np.pi*fc*np.arange(n_bits)/Fs)

print(s-s2)