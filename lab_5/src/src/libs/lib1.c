#include <math.h>
#include "lib.h"

float Derivative(float A, float deltaX){
    float ans = (cosf(A+deltaX) - cosf(A))/deltaX;
    return ans;
}

float E(int x){
    float res = powf((1+1/(float)x), (float)x);
    return res;
}
