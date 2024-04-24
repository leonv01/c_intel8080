#include <stdio.h>

#include "i8080_cpu.h"

int main(){

    i8080_t *cpu = init_i8080();
    if(cpu != NULL){
        /*
               for(int i = 0; i < 0xFF; i++){
                   write_memory(cpu, 0x100, i);

                   emulate_cycle(cpu);
                   cpu->PC = 0x100;
               }
              */
             write_memory(cpu, 0x100, 0xE9);

             cpu->H = 0x12;
             cpu->L = 0x34;
             emulate_cycle(cpu);
        print_state(cpu);
    }
    else {
        printf("Failed to initialize CPU\n");
    }

    return 0;
}
