#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "result.h"

Result::~Result() {}

Result_sum::Result_sum() : value(0) {}

void Result_sum::update_value(u_int16_t new_value) {
    std::unique_lock<std::mutex> lock(this->m);
    this->value += new_value;
}

void Result_sum::print() {
    std::cout << this->value << '\n';
}

Result_mean::Result_mean(int total_rows) : value(0), rows(total_rows) {}

void Result_mean::update_value(u_int16_t new_value) {
    std::unique_lock<std::mutex> lock(this->m);
    this->value += new_value;
}

void Result_mean::print() {
    std::cout << this->value << '/' << this->rows << '\n';
}

Result_max::Result_max() : value(0) {}

void Result_max::update_value(u_int16_t new_value) {
    std::unique_lock<std::mutex> lock(this->m);
    if (new_value > this->value) {
        this->value = new_value;
    }
}

void Result_max::print() {
    std::cout << this->value << '\n';
}

Result_min::Result_min() : value(65535) {}

void Result_min::update_value(u_int16_t new_value) {
    std::unique_lock<std::mutex> lock(this->m);
    if (new_value < this->value) {
        this->value = new_value;
    }
}

void Result_min::print() {
    std::cout << this->value << '\n';
}

