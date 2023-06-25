#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <complex>

using namespace std;

namespace atmc{

#define PI 3.14159265358979f
typedef complex<float> cplx;
typedef vector<cplx> cplxs;

//Reverse bit order(11010001 -> 10001011)
size_t revbit(size_t x, int log2n){ size_t n=0; for(int i=0;i<log2n;i++){ n<<=1; n|=(x&1); x>>=1; } return n; }

//compute fft
cplxs fft(cplxs const& a){
    size_t N = a.size();
    size_t log2n = log2(N);
    const cplx J(0, 1);
    size_t n = 1 << log2n;
    cplxs b(n);
    for(size_t i = 0; i < n; ++i){ b[revbit(i, log2n)] = a[i]; }
    for(int s = 1; s <= log2n; ++s){
        int m = 1 << s;
        int m2 = m >> 1;
        cplx w(1, 0);
        cplx wm = exp(-J * (PI / m2));
        for(int j = 0; j < m2; ++j){
            for(int k = j; k < n; k += m){
                cplx t = w * b[k + m2];
                cplx u = b[k];
                b[k] = u + t;
                b[k + m2] = u - t;
            }
            w *= wm;
        }
    }
    return b;
}

//Smooth data
cplxs smooth_data(cplxs const& data,int radius=1){
    if(radius <= 1)return data;
    auto out_data = data;
    for(int i = 0; i < data.size(); i++){
        cplx sum = 0;
        int cnt = 0;
        for(int k = -radius; k <= radius; k++){
            auto n = k + i;
            if(n < 0 || n >= data.size())continue;
            sum += data[n];
            cnt++;
        }
        out_data[i] = sum * (1.0f / cnt);
    }
    return out_data;
}

// freq+ mag structure
struct fm{ float freq_mhz,mag; };

// copute fft for arbitrary length 1D input data
// smooth_rad = signal filtering radius
// sampling_freq = sampling frequency
// cutoff_freq_mhz = result cutoff frequency (=sampling_freq_mhz/4 if <0)
vector<fm> fft(vector<float> const& data, int smooth_rad, float sampling_freq_mhz, float cutoff_freq_mhz = -1){
    size_t N = data.size();
    size_t log2N = std::ceil(std::log2(N));
    N = 1 << log2N;
    cplxs cplx_data(N);
    for(int i=0;i<N;i++){
        if(i<data.size())cplx_data[i] = cplx(data[i],0);
        else cplx_data[i] = {0,0};
    }
    cplx_data = smooth_data(cplx_data,smooth_rad);
    auto res = fft(cplx_data);
    std::vector<fm> fms(N/2);

    if(cutoff_freq_mhz<0)cutoff_freq_mhz = sampling_freq_mhz/4;
    int n_cutoff=0;
    for(int i = 0; i < N/2; i++){
        float freq_mhz = sampling_freq_mhz * i / N;
        n_cutoff=i;
        if(freq_mhz > cutoff_freq_mhz){
            break;
        }
        float mag = 2 * std::abs(res[i]) / N;
        fms[i] = {freq_mhz,mag};
    }
    fms.resize(n_cutoff);
    return fms;
}

//Read csv data 
vector<float> read_csv(string const& file_path){
    vector<float> data;
    std::ifstream file(file_path);
    while(file){
        float time, voltage;
        char comma;
        file >> time >> comma >> voltage;
        data.emplace_back(voltage);
    }
    file.close();
    return data;
}

//write csv result
void write_csv(string const& file_path, vector<fm> const& fms ){
    std::ofstream ofile(file_path);
    for(int i = 0; i < fms.size(); ++i){
        ofile << fms[i].freq_mhz << "," << fms[i].mag << "\n";
    }
    ofile.close();
}
}

