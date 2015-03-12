#include "compiler_test.h"

int cc=0;
int i=0;


void TComp_expression(TreeNode* node,int c1, int c2)
{
	int cc1_flag=0;
	int cc2_flag=0;
	if (c1==_minus) cc1_flag=1;
	if (c1==_plus)  cc1_flag=0;
	if (c2==_div) cc2_flag=1;
	if (c2==_mul)  cc1_flag=0;
	

	switch (node->nodeType)
	{
	case _number: {  cc++; printf("%d)",cc); printf("PUSH %s\n",node->token->Text); return; }
	case _identf: {  cc++; printf("%d)",cc); printf("GETVAR %s\n",node->token->Text); return; }
	}
	
	TComp_expression(node->branch1,node->nodeType,node->nodeType);
	TComp_expression(node->branch2,node->nodeType,node->nodeType);

	cc++;
	printf("%d)",cc);
	switch (node->nodeType)
	{
	case _plus: 
		{
			if (cc1_flag) goto mi; 
pl:			printf("ADD\n"); break;
		}
	case _minus: 
		{
			if (cc1_flag) goto pl; 
mi:			printf("SUB\n"); break;		
		}
	case _mul: 
		{
			if (cc2_flag) goto di; 
mu:			printf("MUL\n"); break;
		}
	case _div: 
		{
			if (cc2_flag) goto mu; 
di:			printf("DIV\n"); cc2_flag=1; break;
		}
	case _nequal:
	case _equal:  printf("SUB\n"); break;
	case _lt: 
	case _geq: printf("LT\n"); break; 
	case _gt: 
	case _leq: printf("GT\n"); break;
	}

}

void TComp_if(TreeNode* node)
{
	TComp_expression(node->branch1,0,0);

	cc++;
	printf("%d)",cc);
	switch (node->branch1->nodeType)
	{
	case _nequal: printf("JZ CC + BLOCK_LEN\n"); break;
	case _equal: printf("JNZ CC + BLOCK_LEN\n"); break;
	case _lt: 
	case _gt: printf("JNZ CC + BLOCK_LEN\n"); break;
	case _geq: 
	case _leq: printf("JZ CC + BLOCK_LEN\n"); break;
	}

	TComp_statement(node->branch2);
}

void TComp_ifelse(TreeNode* node)
{

	TComp_if(node);

	cc++;
	printf("%d)",cc);
	printf("JMP CC + BLOCK2_LEN\n");
	TComp_statement(node->branch3);
}

void TComp_assigment(TreeNode* node)
{	
	TComp_expression(node->branch2,0,0);

	cc++;
	printf("%d)",cc);
	printf("SETVAR %s\n",node->branch1->token->Text);
}

void TComp_while(TreeNode* node)
{	
	TComp_if(node);

	cc++;
	printf("%d)",cc);
	printf("JMP toOrigin\n");

}

void TComp_print(TreeNode* node)
{
	TComp_expression(node->branch1,0,0);

	cc++;
	printf("%d)",cc);
	printf("PRINT\n");
}

void TComp_statement(TreeNode* node)
{

	switch (node->nodeType)
	{
	case _block: TComp_statements(node); break;
	case _if: TComp_if(node); break;
	case _ifelse: TComp_ifelse(node); break;
	case _assigment:  TComp_assigment(node); break;
	case _while: TComp_while(node); break;
	case _print: TComp_print(node); break;
	}
}

void TComp_statements(TreeNode* node)
{
	i++;
	if (node->branch1) TComp_statement(node->branch1);
	if (node->branch2) TComp_statements(node->branch2);
	i--;
	if(i==0) { 	cc++; printf("%d)",cc); printf("HLT %d\n",cc); }
}

