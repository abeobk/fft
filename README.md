# fft

# How to use:

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
