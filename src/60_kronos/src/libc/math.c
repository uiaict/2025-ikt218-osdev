#include "libc/math.h"

float wrap_angle(float x) {
    while (x > PI)  x -= 2 * PI;
    while (x < -PI) x += 2 * PI;
    return x;
}

float sin(float x) {
    x = wrap_angle(x);
    float x2 = x * x;
    return x
        - (x2 * x) / 6.0
        + (x2 * x2 * x) / 120.0
        - (x2 * x2 * x2 * x) / 5040.0;
}

float cos(float x) {
    x = wrap_angle(x);
    float x2 = x * x;
    return 1.0
        - x2 / 2.0
        + (x2 * x2) / 24.0
        - (x2 * x2 * x2) / 720.0;
}