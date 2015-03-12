#ifndef _COMPILER_H_
#define _COMPILER_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "tree.h"

int _offset();

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

void Comp_expression(TreeNode* node,int c1, int c2);

void Comp_if(TreeNode* node);

void Comp_ifelse(TreeNode* node);

void Comp_assigment(TreeNode* node);

void Comp_while(TreeNode* node);

void Comp_print(TreeNode* node);

void Comp_statement(TreeNode* node);

void Comp_statements(TreeNode* node);

void* Compiler(TreeNode* tree);

#endif