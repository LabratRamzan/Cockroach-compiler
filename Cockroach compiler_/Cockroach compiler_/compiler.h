#ifndef _COMPILER_H_
#define _COMPILER_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "tree.h"

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

typedef struct
{
	char* Vars[256];
	int offset;
	int VarCount;
	int comandCounter;
	int nodeLevel;
}InfoData_Comp;

int _offset();

InfoData_Comp* InfoData_Comp_CR();

void* Compiler(TreeNode* tree,unsigned long*  offset);

void* Comp_expression(TreeNode* node,int c1, int c2, InfoData_Comp* inf, void* byte_code);

void* Comp_if(TreeNode* node, InfoData_Comp* inf, void* byte_code);

void* Comp_ifelse(TreeNode* node, InfoData_Comp* inf, void* byte_code);

void* Comp_assigment(TreeNode* node, InfoData_Comp* inf, void* byte_code);

void* Comp_while(TreeNode* node, InfoData_Comp* inf, void* byte_code);

void* Comp_print(TreeNode* node, InfoData_Comp* inf, void* byte_code);

void* Comp_statement(TreeNode* node, InfoData_Comp* inf, void* byte_code);

void* Comp_statements(TreeNode* node, InfoData_Comp* inf, void* byte_code);

#endif