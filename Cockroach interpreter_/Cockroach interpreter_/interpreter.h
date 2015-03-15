#define _CRT_SECURE_NO_WARNINGS
#define VARMAX 256
#define COMMAX 256
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stack.h"

enum comands{
	POP,	//0
	ADD,	//1
	SUB,	//2
	MUL,	//3
	DIV,	//4
	PRINT,	//5
	HLT,	//6
	LT, // !LT = GEQ
	GT, // !GT = LEQ
	PUSH_,	//9
	SETVAR_,//A
	GETVAR_,//B
	JZ_,	//C
	JNZ_,	//D
	JMP_	//E
};

typedef struct{
	int comand;
	int arg;
}comand;

typedef struct{
	void* instructions;
	unsigned long comandCoounter;
	unsigned long size;
	int vars[VARMAX];
	comand comands[COMMAX];
}Info_Interp;

int getVarOrNum(Info_Interp* info);

void SaveInctructions(Info_Interp* info);

void Interpreter(void* a);

void Execution(Info_Interp* info);