#include "fft.hpp"

using namespace std;
using namespace atmc;

int main(void){
    double dt;
    //read csv data
    auto data = read_csv("./signal.csv");
    //compute fft
    auto res = fft(data,9,1000,50); //smooth = 9, sampling freq = 100mhz, cutoff = 50mhz
    //write result
    write_csv("./res.csv",res);
}

