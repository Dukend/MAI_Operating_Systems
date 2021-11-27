#include "./libs/lib.h"
#include <stdio.h>


int main() {
    int cmd = 0;

    while (scanf("%d", &cmd) != EOF) {
        if(cmd == 1){
            float a, deltaX;
            scanf("%f%f", &a, &deltaX);
            printf("Calculating derivative!\n");
            float res = Derivative(a, deltaX);
            printf("Derivative is %f\n", res);
        }
        else if (cmd == 2){
            int x;
            scanf("%d", &x);
            printf("Calculating E!\n");
            float e = E(x);
            printf("E is %f\n", e);
        }
        else{
            printf("Wrong command!\n");
            return -1;
        }
    }
}