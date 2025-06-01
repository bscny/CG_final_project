#include "object.h"

// the big 3
Object::Object(){
    w_r = 0;
    w_t = 0;
    refractive_index = 1.0; 
}

Object::Object(const Object &in) {
    w_r = in.w_r;
    w_t = in.w_t;
    refractive_index = in.refractive_index;
}

void Object::operator= (const Object &in) {
    w_r = in.w_r;
    w_t = in.w_t;
    refractive_index = in.refractive_index;
}

// other constructor
Object::Object(float in_w_r){
    w_r = in_w_r;
    w_t = 0;
    refractive_index = 1.0;
}

Object::Object(float in_w_r, float in_w_t){
    w_r = in_w_r;
    w_t = in_w_t;
    refractive_index = 1.0;
}

Object::Object(float in_w_r, float in_w_t, float in_refractive_index) {
    w_r = in_w_r;
    w_t = in_w_t;
    refractive_index = in_refractive_index;
}

// getters
float Object::get_w_r() const {
    return w_r;
}

float Object::get_w_t() const {
    return w_t;
}

float Object::get_refractive_index() const {
    return refractive_index;
}

// setters
void Object::set_w_r(float in) {
    w_r = in;
}

void Object::set_w_t(float in) {
    w_t = in;
}

void Object::set_refractive_index(float in) {
    refractive_index = in;
}