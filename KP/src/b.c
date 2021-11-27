#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include <fcntl.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    int pcb = atoi(argv[1]);
    int pab = atoi(argv[2]);  
    
    size_t size;

    while (read(pab, &size, sizeof(size_t)) > 0) {
        printf("[B] - Get A: %zu\n", size);
        read(pcb, &size, sizeof(size_t));
        printf("[B] - Get C: %zu\n", size);
    }

    close(pcb);
    close(pab);

    printf("C\n");
    
    return 0;
}
