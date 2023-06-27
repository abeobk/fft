//Complex nubmer implementation for C
//Do Van Phu (C) 2023
//ATMC.Co.,LTD

#ifndef _C_CPLX_H_
#define _C_CPLX_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


#define PI 3.14159265358979f

/*COMPLEX NUMBER*/
typedef struct cplx{ float re,im; }cplx_t;

extern inline cplx_t cplx_create(float re, float im){ cplx_t c; c.re=re;c.im=im;return c; }

/*complex number operations*/
//addition of two complex numbers
extern inline cplx_t cplx_add(cplx_t* a, cplx_t* b){ return cplx_create(a->re+b->re,a->im+b->im); }
//subtraction of two complex numbers
extern inline cplx_t cplx_sub(cplx_t* a, cplx_t* b){ return cplx_create(a->re-b->re,a->im-b->im); }
//multiplication of two complex numbers
extern inline cplx_t cplx_mul(cplx_t* a, cplx_t* b){ return cplx_create(a->re*b->re - a->im*b->im,a->re*b->im+a->im*b->re); }
//multiply a complex number with a constant
extern inline cplx_t cplx_mulk(cplx_t* a, float k){ return cplx_create(a->re*k,a->im*k); }
//devide a complex number by a constant
extern inline cplx_t cplx_div(cplx_t* a, float k){ return cplx_create(a->re/k,a->im/k); }
//compute magnitude of a complex number
extern inline float cplx_mag(cplx_t* a){return sqrtf(a->re*a->re + a->im*a->im);}

// compute exponent of a complexnumber x+iy: exp(x+iy) = exp(x)*(cos(y)+i*sin(y))
extern inline cplx_t cplx_exp(cplx_t* a){
    float exp_re = expf(a->re);
    float cos_im = cosf(a->im);
    float sin_im = sinf(a->im);
    return cplx_create(exp_re*cos_im, exp_re*sin_im);
}

#endif