#include <math.h>
#include "lib.h"

float Derivative(float A, float deltaX){
    float ans = (cosf(A+deltaX) - cosf(A-deltaX))/(2*deltaX);
    return ans;
}

int factorial(int x) {
    if (x == 0) {
        return 1;
    }
    else if (x == 1) {
        return 1;
    }
    return x * factorial(x - 1);
}
float E(int x){
    float sum = 0;
    for (int i = 0; i < x; ++i) {
        sum += 1. / factorial(i);
    }

    return sum;
}