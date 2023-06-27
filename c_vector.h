//Dynamic vector for C
//Do Van Phu (C) 2023
//ATMC.Co.,LTD
#ifndef _C_VECTOR_H_
#define _C_VECTOR_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*c_vec_t: dynamic array simple implementation*/
typedef struct c_vector{
    uint32_t size;
    uint32_t capacity;
    uint32_t elem_size;
    void* data;
}c_vec_t;

//create an empty vector, return NULL if failed
extern inline c_vec_t* c_vector_create(uint32_t elem_size){ 
    c_vec_t* v = malloc(sizeof(c_vec_t));
    if(v==NULL)return v;
    v->size=0;
    v->elem_size=elem_size;
    v->capacity=0;
    v->data=NULL;
    return v;
}
//create a vector of n (=size) elements, return NULL if failed
extern inline c_vec_t* c_vector_create_with_size(uint32_t elem_size, uint32_t size){ 
    void * data = malloc(size*elem_size);
    if(data==NULL)return NULL;
    c_vec_t* v = malloc(sizeof(c_vec_t));
    if(v==NULL)return v;
    v->size=size;
    v->elem_size=elem_size;
    v->capacity=size;
    v->data=data;
    return v;
}
//copy src->dst return false if failed
extern inline bool c_vector_copy(c_vec_t* dst, c_vec_t* src){
    if(dst==NULL || src==NULL)return false;
    if(dst->capacity*dst->elem_size >= src->size*src->elem_size){
       dst->size = src->size; //match size
    }
    else{
        dst->capacity = dst->size = src->size; //match size and capacity
        dst->data = malloc(src->size*src->elem_size);
        if(dst->data==NULL)return false;
    }
    dst->elem_size = src->elem_size; //match elem size
    memcpy(dst->data, src->data, src->size*src->elem_size);
    return true;
}

//free vector data
extern inline void c_vector_delete(c_vec_t * v){free(v->data); free(v);}

//grow capacity of vector if needed, return false if failed
extern inline bool c_vector_grow(c_vec_t* v, uint32_t need){
    if(v->capacity>=v->size+need)return true;
    uint32_t new_capacity = 3*(v->capacity+need)/2; //grow 1.5x
    if(new_capacity<16)new_capacity=16;
    void* new_data = malloc(new_capacity*v->elem_size);
    if(new_data==NULL)return false;
    memcpy(new_data,v->data,v->size*v->elem_size);
    free(v->data);
    v->data = new_data;
    v->capacity = new_capacity;
    return true;
}

extern inline void c_vector_resize(c_vec_t* v, uint32_t new_size){
    c_vector_grow(v, (new_size <= v->capacity) ? 0 : new_size - v->capacity);
    v->size = new_size;
}

//shrink vector to size to save memory, return false if failed
extern inline bool c_vector_shrink_to_size(c_vec_t* v){
    uint32_t new_capacity = v->size; //grow 1.5x
    void* new_data = malloc(new_capacity*v->elem_size);
    if(new_data==NULL)return false;
    memcpy(new_data,v->data,v->size*v->elem_size);
    free(v->data);
    v->data = new_data;
    v->capacity = new_capacity;
    return true;
}
//add an element to vector, return false if failed
extern inline bool c_vector_push_back(c_vec_t* v, void* elem){
    if(c_vector_grow(v,1)){//grow 1 more slot if needed
        memcpy(v->data + v->size*v->elem_size, elem, v->elem_size);
        v->size+=1;
        return true;
    }
    return false;
}
//add an element to vector, return false if failed
extern inline bool c_vector_pop_back(c_vec_t* v){
    if(v->size > 0){v->size-=1; return true;}
    return false;
}

//get starting address of idx-th element
extern inline void* c_vector_elem(c_vec_t* v, uint32_t idx){return v->data+idx*v->elem_size;}

#endif