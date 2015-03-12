#ifndef _TCOMPILER_H_
#define _TCOMPILER_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"

//enum comands{
//	POP,
//	ADD,
//	SUB,
//	MUL,
//	DIV,
//	HLT,
//	LT, // !LT = GEQ
//	GT, // !GT = LEQ
//	SETVAR_,
//	GETVAR_,
//	PUSH_,
//	JZ_,
//	JNZ_,
//	JMP_
//};

void TComp_expression(TreeNode* node,int c1, int c2);

void TComp_if(TreeNode* node);

void TComp_ifelse(TreeNode* node);

void TComp_assigment(TreeNode* node);

void TComp_while(TreeNode* node);

void TComp_print(TreeNode* node);

void TComp_statement(TreeNode* node);

void TComp_statements(TreeNode* node);

	//out_program=(char*)realloc( out_program,sizeof(char)*(7+(int)strlen(node->branch1->token->Text)));
	//strcat(out_program,"SETVAR ");

#endif