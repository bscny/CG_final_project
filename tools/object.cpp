#include "object.h"

// the big 3
Object::Object(){
    w_r = 0;
    w_t = 0;
    reflective_index = 1.0; 
}

Object::Object(const Object &in) {
    w_r = in.w_r;
    w_t = in.w_t;
    reflective_index = in.reflective_index;
}

void Object::operator= (const Object &in) {
    w_r = in.w_r;
    w_t = in.w_t;
    reflective_index = in.reflective_index;
}

// other constructor
Object::Object(float in_w_r){
    w_r = in_w_r;
    w_t = 0;
    reflective_index = 1.0;
}

Object::Object(float in_w_r, float in_w_t){
    w_r = in_w_r;
    w_t = in_w_t;
    reflective_index = 1.0;
}

Object::Object(float in_w_r, float in_w_t, float in_reflective_index) {
    w_r = in_w_r;
    w_t = in_w_t;
    reflective_index = in_reflective_index;
}

// getters
float Object::get_w_r() const {
    return w_r;
}

float Object::get_w_t() const {
    return w_t;
}

float Object::get_reflective_index() const {
    return reflective_index;
}

// setters
void Object::set_w_r(float in) {
    w_r = in;
}

void Object::set_w_t(float in) {
    w_t = in;
}

void Object::set_reflective_index(float in) {
    reflective_index = in;
}