#include "cfft.h"

int main(void){
    //read csv data
    float_vec_t* data = read_csv("./signal.csv");
    if(data==NULL)return -1; //fail 1
    //compute fft
    fm_vec_t*res = fft_1d(data,9,1000,50); //smooth = 9, sampling freq = 1000MHz (1GHz), cutoff = 50mhz
    c_vector_delete(data);
    if(res==NULL)return -2; //fail 2
    //write result
    write_csv("./res.csv",res);
    c_vector_delete(res);
    return 0;
}

