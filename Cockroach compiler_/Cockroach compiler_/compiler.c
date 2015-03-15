#include "compiler.h"

InfoData_Comp* InfoData_Comp_CR()
{
	InfoData_Comp* info = (InfoData_Comp*)malloc(sizeof(InfoData_Comp));
	info->comandCounter=0;
	info->nodeLevel=0;
	info->offset=0;
	info->VarCount=0;
	return info;
}

void* Compiler(TreeNode* tree, int* offset)
{
	void* byte_code = malloc(0);
	InfoData_Comp* info = InfoData_Comp_CR();
	byte_code=Comp_statements(tree, info, byte_code);
	*offset=info->offset;
	free(info);
	return byte_code;
}

int checkVar(char* var, InfoData_Comp* inf, void* byte_code)
{
	int i,g; 
	for (i=0; i<inf->VarCount; i++)
	{
		if (!strcmp(var,inf->Vars[i])) break;
	}
	if (i<inf->VarCount) return i;
	inf->Vars[inf->VarCount]=var;
	inf->VarCount++;
	return i;
}

void* setCodeB(enum comands com, char* t, InfoData_Comp* inf, void* byte_code)
{
	int v;
	byte_code=realloc(byte_code,5+inf->offset);
	v=com;
	memcpy((char*)byte_code+inf->offset,&v,1);
	if (isalpha(*t)) 
	{
		v=checkVar(t,inf,byte_code);
		memcpy((char*)byte_code+1+inf->offset,&v,4);
	}
	else
	{
		v=atoi(t);
		memcpy((char*)byte_code+1+inf->offset,&v,4);
	}
	inf->offset+=5;
	return byte_code;
}

void* setCodeU(enum comands com, InfoData_Comp* inf, void* byte_code)
{
	int v=com;
	byte_code=(char*)realloc(byte_code,1+inf->offset);
	memcpy((char*)byte_code+inf->offset,(char*)&v,1);
	inf->offset++;
	return byte_code;
}

void* Comp_expression(TreeNode* node,int c1, int c2, InfoData_Comp* inf, void* byte_code)
{
	int c1_flag=0;
	int c2_flag=0;
	if (c1==_minus) c1_flag=1;
	if (c1==_plus)  c1_flag=0;
	if (c2==_div) c2_flag=1;
	if (c2==_mul)  c1_flag=0;

	switch (node->nodeType)
	{
	case _number: (inf->comandCounter++); byte_code=setCodeB(PUSH_,node->token->Text,inf,byte_code);	 return byte_code; ;
	case _identf: (inf->comandCounter++); byte_code=setCodeB(GETVAR_,node->token->Text,inf,byte_code); return byte_code;;
	}

	byte_code= Comp_expression(node->branch1,node->nodeType,node->nodeType, inf, byte_code);
	byte_code= Comp_expression(node->branch2,node->nodeType,node->nodeType, inf, byte_code);

	(inf->comandCounter)++;

	switch (node->nodeType)
	{
	case _plus: 
		{
			if (c1_flag) goto mi; 
pl:			byte_code= setCodeU(ADD,inf,byte_code); break;
		}
	case _minus: 
		{
			if (c1_flag) goto pl; 
mi:			byte_code= setCodeU(SUB,inf,byte_code); break;		
		}
	case _mul: 
		{
			if (c2_flag) goto di; 
mu:			byte_code= setCodeU(MUL,inf,byte_code); break;
		}
	case _div: 
		{
			if (c2_flag) goto mu; 
di:			byte_code= setCodeU(DIV,inf,byte_code); break;
		}
	case _equal: byte_code= setCodeU(SUB,inf,byte_code); break;
	case _lt: 
	case _geq: byte_code= setCodeU(LT,inf,byte_code); break; 
	case _gt: 
	case _leq: byte_code= setCodeU(GT,inf,byte_code); break;
	}
	return byte_code;
}

void* Comp_if(TreeNode* node, InfoData_Comp* inf, void* byte_code)
{
	int temp1,temp=0;
	
	byte_code= Comp_expression(node->branch1,0,0,inf, byte_code);

	temp=inf->offset;
	byte_code=realloc(byte_code,inf->offset+5);
	inf->offset+=5;
	
	inf->comandCounter++;
	byte_code= Comp_statement(node->branch2,inf, byte_code);

	switch (node->branch1->nodeType)
	{
	case _lt: 
	case _gt: 
	case _nequal: temp1=JNZ_; memcpy((char*)byte_code+temp,(char*)&temp1,1); break;
	case _geq: 
	case _leq: 
	case _equal: temp1=JZ_; memcpy((char*)byte_code+temp,(char*)&temp1,1); break;
	}
	
	inf->comandCounter++;
	temp++;
	memcpy((char*)byte_code+temp,&(inf->comandCounter),4);
	inf->comandCounter--;

	return byte_code;
}

void* Comp_ifelse(TreeNode* node, InfoData_Comp* inf, void* byte_code)
{
	int temp1,temp=0;

	byte_code= Comp_expression(node->branch1,0,0,inf, byte_code);

	temp=inf->offset;
	byte_code=realloc(byte_code,inf->offset+5);
	inf->offset+=5;

	inf->comandCounter++;
	byte_code= Comp_statement(node->branch2,inf, byte_code);

	switch (node->branch1->nodeType)
	{
	case _lt: 
	case _gt: 
	case _nequal: temp1=JNZ_; memcpy((char*)byte_code+temp,(char*)&temp1,1); break;
	case _geq: 
	case _leq: 
	case _equal: temp1=JZ_; memcpy((char*)byte_code+temp,(char*)&temp1,1); break;
	}
	
	inf->comandCounter+=2;
	temp++;
	memcpy((char*)byte_code+temp,&(inf->comandCounter),4);
	inf->comandCounter-=2;

	temp=inf->offset;
	byte_code=realloc(byte_code,inf->offset+5);
	inf->offset+=5;

	inf->comandCounter++;
	byte_code= Comp_statement(node->branch3, inf, byte_code);
	
	inf->comandCounter++;
	temp1=JMP_;
	memcpy((char*)byte_code+temp,(char*)&temp1,1);
	temp++;
	memcpy((char*)byte_code+temp,&(inf->comandCounter),4);
	inf->comandCounter--;
	return byte_code;
}

void* Comp_assigment(TreeNode* node,InfoData_Comp* inf, void* byte_code)
{	
	byte_code= Comp_expression(node->branch2,0,0,inf, byte_code);
	(inf->comandCounter)++;
	byte_code= setCodeB(SETVAR_,node->branch1->token->Text,inf, byte_code);
	return byte_code;
}

void* Comp_while(TreeNode* node, InfoData_Comp* inf, void* byte_code)
{	
	int temp1,temp=0;
	
	inf->comandCounter++;
	temp=inf->comandCounter;
	byte_code= Comp_if(node,inf, byte_code);
	inf->comandCounter--;

	inf->comandCounter++;
	temp1=JMP_;
	byte_code=realloc(byte_code,inf->offset+5);
	memcpy((char*)byte_code+inf->offset,(char*)&temp1,1);
	memcpy((char*)byte_code+1+inf->offset,&temp,4);
	inf->offset+=5;
	return byte_code;

}

void* Comp_print(TreeNode* node, InfoData_Comp* inf, void* byte_code)
{
	byte_code= Comp_expression(node->branch1,0,0,inf, byte_code);
	inf->comandCounter++;
	byte_code= setCodeU(PRINT,inf, byte_code);
	return byte_code;
}

void* Comp_break(TreeNode* node, InfoData_Comp* inf, void* byte_code)
{

}

void* Comp_statement(TreeNode* node,InfoData_Comp* inf, void* byte_code)
{

	switch (node->nodeType)
	{
	case _block: byte_code= Comp_statements(node, inf, byte_code); break;
	case _if: byte_code= Comp_if(node,inf, byte_code); break;
	case _ifelse: byte_code= Comp_ifelse(node,inf, byte_code); break;
	case _assigment:  byte_code= Comp_assigment(node, inf, byte_code); break;
	case _while: byte_code= Comp_while(node,inf, byte_code); break;
	case _print: byte_code= Comp_print(node,inf, byte_code); break;
	case _break: byte_code= Comp_break(node,inf, byte_code); break;
	}
	return byte_code;
}

void* Comp_statements(TreeNode* node,InfoData_Comp* inf, void* byte_code)
{
	(inf->nodeLevel)++;
	if (node->branch1) byte_code= Comp_statement(node->branch1, inf, byte_code);
	if (node->branch2) byte_code= Comp_statements(node->branch2, inf, byte_code);
	(inf->nodeLevel)--;
	if(inf->nodeLevel==0) byte_code=setCodeU(HLT,inf, byte_code);
	return byte_code;
}
