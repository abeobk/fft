//FFT implementation for C
//Do Van Phu (C) 2023
//ATMC.Co.,LTD

#ifndef _C_FFT_H_
#define _C_FFT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "c_vector.h"
#include "cplx.h"

// freq+ mag structure
typedef struct fm{ float freq_mhz,mag; }fm_t;
typedef c_vec_t cplx_vec_t;
typedef c_vec_t float_vec_t;
typedef c_vec_t fm_vec_t;

//Reverse bit order(11010001 -> 10001011)
uint32_t revbit(uint32_t x, int log2n){ uint32_t n=0; for(int i=0;i<log2n;i++){ n<<=1; n|=(x&1); x>>=1; } return n; }

//compute fft, return NULL if failed
cplx_vec_t* fft(cplx_vec_t* a){
    if(a==NULL)return NULL;
    cplx_vec_t *b = c_vector_create_with_size(sizeof(cplx_t),a->size);
    if(b==NULL)return NULL;
    if(!c_vector_copy(b,a))return NULL;
    uint32_t N = a->size;
    uint32_t log2n = log2f(N);
    cplx_t _J = cplx_create(0,-1);
    uint32_t n = 1 << log2n;
    cplx_t* a_ptr = (cplx_t*)a->data;
    cplx_t* b_ptr = (cplx_t*)b->data;
    for(uint32_t i = 0; i < n; ++i){ b_ptr[revbit(i, log2n)] = a_ptr[i]; }
    for(int s = 1; s <= log2n; ++s){
        int m = 1 << s;
        int m2 = m >> 1;
        cplx_t w = cplx_create(1,0);
        cplx_t exp_term = cplx_mulk(&_J, PI/m2);
        cplx_t wm = cplx_exp(&exp_term);
        for(int j = 0; j < m2; ++j){
            for(int k = j; k < n; k += m){
                cplx_t t = cplx_mul(&w , &b_ptr[k + m2]);
                cplx_t u = b_ptr[k];
                b_ptr[k] = cplx_add(&u , &t);
                b_ptr[k + m2] = cplx_sub(&u , &t);
            }
            w=cplx_mul(&w,&wm);
        }
    }
    return b;
}

//Smooth data, return NULL if failed
cplx_vec_t* smooth_data(cplx_vec_t* data,int radius){
    if(data==NULL)return NULL;
    cplx_vec_t *out_data = c_vector_create_with_size(sizeof(cplx_t), data->size);
    if(out_data==NULL)return NULL;
    if(!c_vector_copy(out_data, data))return NULL;
    if(radius <= 1)return out_data;
    uint32_t N = data->size;
    cplx_t* data_ptr = (cplx_t*)data->data;
    cplx_t* out_data_ptr = (cplx_t*)out_data->data;
    for(int i = 0; i < N; i++){
        cplx_t sum =cplx_create(0,0);
        int cnt = 0;
        for(int k = -radius; k <= radius; k++){
            int n = k + i;
            if(n < 0 || n >= N)continue;
            sum = cplx_add(&sum, &data_ptr[n]);
            cnt++;
        }
        out_data_ptr[i] = cplx_mulk(&sum, (1.0f / cnt));
    }
    return out_data;
}

// copute fft for arbitrary length 1D input data, return NULL if failed
// smooth_rad = signal filtering radius
// sampling_freq = sampling frequency
// cutoff_freq_mhz = result cutoff frequency (=sampling_freq_mhz/4 if <0)
// input: data is vector of float
// output: return vector of fm
float_vec_t* fft_1d(float_vec_t* data, int smooth_rad, float sampling_freq_mhz, float cutoff_freq_mhz){
    if(data==NULL)return NULL;
    uint32_t log2N = ceilf(log2f(data->size));
    uint32_t N = 1 << log2N;
    cplx_vec_t* cplx_data =  c_vector_create_with_size(sizeof(cplx_t),N);
    if(cplx_data == NULL)return NULL;

    cplx_t* cplx_data_ptr = (cplx_t*)cplx_data->data;
    float* data_ptr = (float*)data->data;
    for(int i=0;i<N;i++){
        if(i<data->size){ cplx_data_ptr[i] = cplx_create(data_ptr[i],0);}
        else { cplx_data_ptr[i] = cplx_create(0,0); }
    }

    cplx_vec_t* smooth_cplx_data = smooth_data(cplx_data, smooth_rad);
    c_vector_delete(cplx_data);
    if(smooth_cplx_data==NULL)return NULL;

    cplx_vec_t* res = fft(smooth_cplx_data);
    c_vector_delete(smooth_cplx_data);
    if(res==NULL)return NULL;

    fm_vec_t* fms = c_vector_create_with_size(sizeof(fm_t),N/2);

    uint32_t n_cutoff=0;
    if(fms!=NULL){
        cplx_t* res_ptr = (cplx_t*)res->data;
        fm_t* fms_ptr = (fm_t*)fms->data;

        if(cutoff_freq_mhz<0)cutoff_freq_mhz = sampling_freq_mhz/4;
        for(uint32_t i = 0; i < N/2; i++){
            float freq_mhz = sampling_freq_mhz * i / N;
            n_cutoff=i;
            if(freq_mhz > cutoff_freq_mhz){ break; }
            float mag = 2 * cplx_mag(&res_ptr[i]) / N;
            fms_ptr[i].freq_mhz = freq_mhz;
            fms_ptr[i].mag= mag;
        }
    }

    c_vector_delete(res);
    c_vector_resize(fms,n_cutoff);
    c_vector_shrink_to_size(fms);
    return fms;
}

//Read csv data 
float_vec_t* read_csv(char const* file_path){
    FILE* fp;
    fp = fopen(file_path, "r");
    if(fp==NULL){
        printf("Failed to open file for reading");
        return NULL;
    }
    float_vec_t * data = c_vector_create(sizeof(float));
    if(data==NULL)return NULL;
    float time, voltage;
    char comma;
    while(!feof(fp)){
        fscanf(fp, "%f%c%f",&time,&comma,&voltage);
        c_vector_push_back(data, &voltage);
    }
    fclose(fp);
    return data;
}

//write csv result
bool write_csv(const char* file_path, fm_vec_t* fms){
    if(fms==NULL)return false;
    FILE* fp;
    fp = fopen(file_path, "w");
    if(fp==NULL){
        printf("Failed to open file for writing");
        return NULL;
    }
    fm_t* fms_ptr = (fm_t*)fms->data;
    for(int i = 0; i < fms->size; ++i){
        fprintf(fp,"%f,%f\n",fms_ptr[i].freq_mhz, fms_ptr[i].mag);
    }
    fclose(fp);
    return true;
}


#endif


