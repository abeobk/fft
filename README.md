# fft

# How to use:

CPP
  ```cpp
  struct fm{float freq_mhz,mag};
  //compute fft
  //data: input data is vector<float>
  //smooth_radius: smooth radius (simple moving average smooth)
  //sampling_freq_mhz: data sampling frequency
  //cutoff_freq_mhz: maximum frequency in result
  //res: fft result is vector<fm>
  res = fft(data, smooth_radius, sampling_freq_mhz, cutoff_freq_mhz);
  ```

# How to compile:

   ```bash  
  g++ test.cpp -o test
  ./test
   ```


C
  ```cpp
  //vm_vec_t: c_vec_t
  //data: float_vec_t
  //please check c_vector.h for detail implementation
  fm_vec_t * res = fft_1d(data, smooth_radius, sampling_freq_mhz, cutoff_freq_mhz);
  ```

# How to compile:

   ```bash  
  gcc test_fft.c -o test_fft -lm
  ./test_fft
   ```

