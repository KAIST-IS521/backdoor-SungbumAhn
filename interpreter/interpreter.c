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

void halt (struct VMcontext* ctx, const uint32_t instr){

	is_running = false;
	printf("Terminate the proccess  \n");
}

void load (struct VMcontext* ctx, const uint32_t instr){

	const uint8_t r0 = EXTRACT_B1(instr);
	const uint8_t r1 = EXTRACT_B2(instr);
	const uint32_t addr = ctx->r[r1].value;
	ctx->[r0].value = 0x000000ff & read_mem(ctx, addr);

        printf("Load a value  \n");
	
}

void store (struct VMcontext* ctx, const uint32_t instr){

	const uint8_t r0 = EXTRACT_B1(instr);
	const uint8_t r1 = EXTRACT_B2(instr);
	const uint32_t addr = ctx->r[r0].value;
	write_mem(ctx, addr, EXTRACT_B0(ctx->[r1].value));

        printf("Store \n");

}

void move (struct VMcontext* ctx, const uint32_t instr){

	const uint8_t r0 = EXTRACT_B1(instr);
	const uint8_t r1 = EXTRACT_B2(instr);
	ctx->r[r0].value = ctx->r[r1].value;
	
        printf("Move  \n");
}

void puti (struct VMcontext* ctx, const uint32_t instr){

	const uint8_t r0 = EXTRACT_B1(instr);
	const uint8_t imm = EXTRACT_B2(instr);
	ctx->r[r0].value = 0x000000ff & imm;

        printf("Puti  \n");
}

void add (struct VMcontext* ctx, const uint32_t instr){

	const uint8_t r0 = EXTRACT_B1(instr);
	const uint8_t r1 = EXTRACT_B2(instr);
        const uint8_t r2 = EXTRACT_B3(instr);
	ctx->r[r0].value = ctx->r[r1].value + ctx->r[r2].value;

        printf("Add  \n");
}

void sub (struct VMcontext* ctx, const uint32_t instr){

	const uint8_t r0 = EXTRACT_B1(instr);
	const uint8_t r1 = EXTRACT_B2(instr);
        const uint8_t r2 = EXTRACT_B3(instr);
	ctx->r[r0].value = ctx->r[r1].value - ctx->r[r2].value;

	printf("Sub  \n");
}

void gt (struct VMcontext* ctx, const uint32_t instr){

	const uint8_t r0 = EXTRACT_B1(instr);
	const uint8_t r1 = EXTRACT_B2(instr);
        const uint8_t r2 = EXTRACT_B3(instr);
	if (ctx->r[r1].value > ctx->r[r2].value)
	    ctx->r[r0].value = 1;
	else
	ctx->r[r0].value = 0;

        printf("Gt  \n");
}

void ge (struct VMcontext* ctx, const uint32_t instr){

	const uint8_t r0 = EXTRACT_B1(instr);
	const uint8_t r1 = EXTRACT_B2(instr);
        const uint8_t r2 = EXTRACT_B3(instr);
	if (ctx->r[r1].value >= ctx->r[r2].value)
	    ctx->r[r0].value = 1;
	else
	ctx->r[r0].value = 0;

        printf("Ge  \n");
}

void eq (struct VMcontext* ctx, const uint32_t instr){

	const uint8_t r0 = EXTRACT_B1(instr);
	const uint8_t r1 = EXTRACT_B2(instr);
        const uint8_t r2 = EXTRACT_B3(instr);
	if (ctx->r[r1].value == ctx->r[r2].value)
	    ctx->r[r0].value = 1;
	else
	ctx->r[r0].value = 0;

        printf("Eq  \n");
}

void ite (struct VMcontext* ctx, const uint32_t instr){

	const uint8_t r0 = EXTRACT_B1(instr);
	const uint8_t imm1 = EXTRACT_B2(instr);
	const uint8_t imm2 = EXTRACT_B3(instr);
	if (ctx->r[r0].value > 0)
	    ctx->pc = imm1;
	else
	ctx->pc = imm2;

        printf("Ite  \n");	
}

void jump (struct VMcontext* ctx, const uint32_t instr){

	const uint8_t imm = EXTRACT_B1(instr);
	ctx->pc = imm;

	printf("Jump  \n");
}

void puts (struct VMcontext* ctx, const uint32_t instr){

	const uint8_t r0 = EXTRACT_B1(instr);
	uint32_t addr = ctx->r[r0].value;
        uint8_t val;
	while(true) {
		val = read_mem(ctx, addr);
		if (val) {
			putchar(val);
			addr++;
		}
		else
			break;
		}

	printf("puts  \n");
}

void gets (struct VMcontext* ctx, const uint32_t instr){

	const uint8_t r0 = EXTRACT_B1(instr);
	uint32_t addr = ctx->r[r0].value;
        uint8_t val;
	while(true) {
		val = (uint8_t) getchar();
		if (val == '\n') break;
		write_mem(ctx, addr, val);
		addr++;

        printf("gets  \n");
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
