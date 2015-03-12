#include "compiler.h"

void *bc, *bc_temp;
char* Vars[256];

int offset=0;
int VarCount=0;
int comandCounter=0;
int nodeLevel=0;

void* Compiler(TreeNode* tree)
{
	Comp_statements(tree);
	return bc;
}

int _offset()
{ return offset; }

int checkVar(char* var)
{
	int i,g; 
	for (i=0; i<VarCount; i++)
	{
		if (!strcmp(var,Vars[i])) break;
	}
	if (i<VarCount) return i;
	Vars[VarCount]=var;
	VarCount++;
	return i;
}

void setCodeB(enum comands com, char* t)
{
	int v;
	bc=realloc(bc,5+offset);
	v=com;
	memcpy((char*)bc+offset,&v,1);
	if (isalpha(*t)) 
	{
		v=checkVar(t);
		memcpy((char*)bc+1+offset,&v,4);
	}
	else
	{
		v=atoi(t);
		memcpy((char*)bc+1+offset,&v,4);
	}
	offset+=5;
}

void setCodeU(enum comands com)
{
	int v=com;
	bc=(char*)realloc(bc,1+offset);
	memcpy((char*)bc+offset,(char*)&v,1);
	offset++;
}

void Comp_expression(TreeNode* node,int c1, int c2)
{
	int c1_flag=0;
	int c2_flag=0;
	if (c1==_minus) c1_flag=1;
	if (c1==_plus)  c1_flag=0;
	if (c2==_div) c2_flag=1;
	if (c2==_mul)  c1_flag=0;

	switch (node->nodeType)
	{
	case _number: comandCounter++; setCodeB(PUSH_,node->token->Text); return;
	case _identf: comandCounter++; setCodeB(GETVAR_,node->token->Text); return;
	}

	Comp_expression(node->branch1,node->nodeType,node->nodeType);
	Comp_expression(node->branch2,node->nodeType,node->nodeType);

	comandCounter++;

	switch (node->nodeType)
	{
	case _plus: 
		{
			if (c1_flag) goto mi; 
pl:			setCodeU(ADD); break;
		}
	case _minus: 
		{
			if (c1_flag) goto pl; 
mi:			setCodeU(SUB); break;		
		}
	case _mul: 
		{
			if (c2_flag) goto di; 
mu:			setCodeU(MUL); break;
		}
	case _div: 
		{
			if (c2_flag) goto mu; 
di:			setCodeU(DIV); break;
		}
	case _equal: setCodeU(SUB); break;
	case _lt: 
	case _geq: setCodeU(LT); break; 
	case _gt: 
	case _leq: setCodeU(GT); break;
	}
}

void Comp_if(TreeNode* node)
{
	int temp1,temp=0;
	
	Comp_expression(node->branch1,0,0);

	temp=offset;
	bc=realloc(bc,offset+5);
	offset+=5;
	
	comandCounter++;
	Comp_statement(node->branch2);

	switch (node->branch1->nodeType)
	{
	case _lt: 
	case _gt: 
	case _nequal: temp1=JNZ_; memcpy((char*)bc+temp,(char*)&temp1,1); break;
	case _geq: 
	case _leq: 
	case _equal: temp1=JZ_; memcpy((char*)bc+temp,(char*)&temp1,1); break;
	}
	
	comandCounter++;
	temp++;
	memcpy((char*)bc+temp,&comandCounter,4);
	comandCounter--;
}

void Comp_ifelse(TreeNode* node)
{
	int temp1,temp=0;

	Comp_expression(node->branch1,0,0);

	temp=offset;
	bc=realloc(bc,offset+5);
	offset+=5;

	comandCounter++;
	Comp_statement(node->branch2);

	switch (node->branch1->nodeType)
	{
	case _lt: 
	case _gt: 
	case _equal: temp1=JNZ_; memcpy((char*)bc+temp,(char*)&temp1,1); break;
	case _geq: 
	case _leq: 
	case _nequal: temp1=JZ_; memcpy((char*)bc+temp,(char*)&temp1,1); break;
	}
	
	comandCounter+=2;
	temp++;
	memcpy((char*)bc+temp,&comandCounter,4);
	comandCounter-=2;

	temp=offset;
	bc=realloc(bc,offset+5);
	offset+=5;

	comandCounter++;
	Comp_statement(node->branch3);
	
	comandCounter++;
	temp1=JMP_;
	memcpy((char*)bc+temp,(char*)&temp1,1);
	temp++;
	memcpy((char*)bc+temp,&comandCounter,4);
	comandCounter--;
}

void Comp_assigment(TreeNode* node)
{	
	Comp_expression(node->branch2,0,0);
	comandCounter++;
	setCodeB(SETVAR_,node->branch1->token->Text);
}

void Comp_while(TreeNode* node)
{	
	int temp1,temp=0;
	
	comandCounter++;
	temp=comandCounter;
	Comp_if(node);
	comandCounter--;

	comandCounter++;
	temp1=JMP_;
	bc=realloc(bc,offset+5);
	memcpy((char*)bc+offset,(char*)&temp1,1);
	memcpy((char*)bc+1+offset,&temp,4);
	offset+=5;

}

void Comp_print(TreeNode* node)
{
	Comp_expression(node->branch1,0,0);
	comandCounter++;
	setCodeU(PRINT);
}

void Comp_break(TreeNode* node)
{

}

void Comp_statement(TreeNode* node)
{

	switch (node->nodeType)
	{
	case _block: Comp_statements(node); break;
	case _if: Comp_if(node); break;
	case _ifelse: Comp_ifelse(node); break;
	case _assigment:  Comp_assigment(node); break;
	case _while: Comp_while(node); break;
	case _print: Comp_print(node); break;
	case _break: Comp_break(node); break;
	}
}

void Comp_statements(TreeNode* node)
{
	nodeLevel++;
	if (node->branch1) Comp_statement(node->branch1);
	if (node->branch2) Comp_statements(node->branch2);
	nodeLevel--;
	if(nodeLevel==0) setCodeU(HLT);
}
