#pragma once


template <typename T>
T gasesteMaxim(T a, T b) {
    return (a > b) ? a : b;
}


template <typename T>
T constringeValoare(T val, T min, T max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}