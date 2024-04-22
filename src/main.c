#include <stdio.h>

#include "i8080_cpu.h"

int main(){

    i8080_t *cpu = init_i8080();
    if(cpu != NULL){

        print_state(cpu);
    }
    else {
        printf("Failed to initialize CPU\n");
    }

    return 0;
}
