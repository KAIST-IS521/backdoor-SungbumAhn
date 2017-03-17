// This is where you put your VM code.
// I am trying to follow the coding style of the original.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "minivm.h"

#define NUM_REGS   (256)
#define NUM_FUNCS  (256)

// Global variable that indicates if the process is running.
static bool is_running = true;

void usageExit() {
    // TODO: show usage
    printf(" Usage :./interpreter <File> \n");
    exit(1);
}

void *halt (struct VMcontext* ctx, const uint32_t instr){
	
	printf("Terminate the proccess  \n");
}

void *load (struct VMcontext* ctx, const uint32_t instr){

        printf("Terminate the proccess  \n");
}

void *store (struct VMcontext* ctx, const uint32_t instr){

        printf("Terminate the proccess  \n");
}

void *move (struct VMcontext* ctx, const uint32_t instr){

        printf("Terminate the proccess  \n");
	}

void *puti (struct VMcontext* ctx, const uint32_t instr){

        printf("Terminate the proccess  \n");
	}

void *add (struct VMcontext* ctx, const uint32_t instr){

        printf("Terminate the proccess  \n");
	}
void *sub (struct VMcontext* ctx, const uint32_t instr){

        printf("Terminate the proccess  \n");
	}

void *gt (struct VMcontext* ctx, const uint32_t instr){

        printf("Terminate the proccess  \n");
	}

void *ge (struct VMcontext* ctx, const uint32_t instr){

        printf("Terminate the proccess  \n");
	}
void *eq (struct VMcontext* ctx, const uint32_t instr){

        printf("Terminate the proccess  \n");
	}
void *ite (struct VMcontext* ctx, const uint32_t instr){

        printf("Terminate the proccess  \n");
	}

void *jump (struct VMcontext* ctx, const uint32_t instr){

        printf("Terminate the proccess  \n");
	}
void *puts (struct VMcontext* ctx, const uint32_t instr){

        printf("Terminate the proccess  \n");
	}
void *gets (struct VMcontext* ctx, const uint32_t instr){

        printf("Terminate the proccess  \n");
	}


void initFuncs(FunPtr *f, uint32_t cnt) {
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        f[i] = NULL;
    }

    // TODO: initialize function pointers
     f[0x00] = halt;
     f[0x10] = load;
     f[0x20] = store;
     f[0x30] = move;
     f[0x40] = puti;
     f[0x50] = add;
     f[0x60] = sub;
     f[0x70] = gt;
     f[0x80] = ge;
     f[0x90] = eq;
     f[0xa0] = ite;
     f[0xb0] = jump;
     f[0xc0] = puts;
     f[0xd0] = gets;
    
}

void initRegs(Reg *r, uint32_t cnt)
{
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        r[i].type = 0;
        r[i].value = 0;
    }
}

int main(int argc, char** argv) {
    VMContext vm;
    Reg r[NUM_REGS];
    FunPtr f[NUM_FUNCS];
    FILE* bytecode;
    uint32_t* pc;

    // There should be at least one argument.
    if (argc < 2) usageExit();

    // Initialize registers.
    initRegs(r, NUM_REGS);
    // Initialize interpretation functions.
    initFuncs(f, NUM_FUNCS);
    // Initialize VM context.
    initVMContext(&vm, NUM_REGS, NUM_FUNCS, r, f);

    // Load bytecode file
    bytecode = fopen(argv[1], "rb");
    if (bytecode == NULL) {
        perror("fopen");
        return 1;
    }

    while (is_running) {
        // TODO: Read 4-byte bytecode, and set the pc accordingly
        stepVMContext(&vm, &pc);
    }

    fclose(bytecode);

    // Zero indicates normal termination.
    return 0;
}
