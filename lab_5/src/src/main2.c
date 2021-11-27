#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>


typedef enum {
    FIRST,
    SECOND,
} CONTEXT;

CONTEXT r = FIRST;

const char* libName1 = "lib1.so";
const char* libName2 = "lib2.so";

float (*Derivative)(float, float) = NULL;
float (*E)(int x) = NULL;
char *err;

void *libHandle = NULL;

void loadLibs(CONTEXT context){
    const char *name;
    if(context == FIRST){
        name = libName1;
    } else{
        name = libName2;
    }

    libHandle = dlopen(name, RTLD_LAZY);
    if(!libHandle){
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
}

void unloadLibs(){
    dlclose(libHandle);
}

void loadContext(){
    loadLibs(r);
    Derivative = (float (*)(float, float))dlsym(libHandle, "Derivative");
    E = (float (*)(int))dlsym(libHandle, "E");
    if((err = dlerror())) {
        fprintf(stderr, "%s\n", err);
        exit(EXIT_FAILURE);
    }
}

void changeContext(){
    unloadLibs();
    if(r == FIRST){
        r = SECOND;
    } else {
        r = FIRST;
    }

    loadContext();
}

int main(){
    r = FIRST;
    loadContext();
    int cmd = 0;
    while (scanf("%d", &cmd) > 0){
        if(cmd == 0){
            changeContext();
            printf("Ok. Contract was changed\n");
            if(r == FIRST){
                printf("Now context is first\n");
            } else{
                printf("Now context is second\n");
            }
            continue;
        }
        else if(cmd == 1){
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
    unloadLibs();
}