# fft

How to use:

```
res = fft(data, smooth_radius, sampling_freq_mhz, cutoff_freq_mhz);
```

- data: input data is vector<float>
- smooth_radius: smooth radius (simple moving average smooth)
- sampling_freq_mhz: data sampling frequency
- cutoff_freq_mhz: maximum frequency in result
- res: fft result is vector<fm>

```
struct fm{float freq_mhz,mag};
```
