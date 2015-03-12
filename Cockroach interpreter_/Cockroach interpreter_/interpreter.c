#include "interpreter.h"

int comandCoounter;
double regA, regB;
void* instructions;
char* ss;
int size;
int vars[VARMAX];
comand comands[COMMAX];

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

int getVarOrNum()
{
	return comands[comandCoounter].arg;
}

void SaveInctructions()
{
	int i=1;
	comand* com;
	com=(comand*)malloc(8);
	while(1)
	{
		com->comand=0;
		memcpy(&(com->comand),instructions,1);
		instructions=(char*)instructions+1;
		if (com->comand>8) 
		{
			memcpy(&(com->arg),instructions,4);
			instructions=(char*)instructions+4;
		}
		comands[i]=*com;
		i++;
		if(com->comand==HLT) break;
	}
}

void Interpreter(void* a)
{
	instructions=a;
	ss=(char*)a;
	SaveInctructions();
	Execution();
}

void Execution()
{
	int com=0;
	int v=0;
	Stack_Init();
	comandCoounter=1;
	while (1)
	{
		com=comands[comandCoounter].comand;
		v=getVarOrNum();
		comandCoounter++;
		switch (com)
		{
		case POP: Pop();
		case ADD: regA=Pop(); regB=Pop(); regB+=regA; Push(regB); break;
		case SUB: regA=Pop(); regB=Pop(); regB-=regA; Push(regB); break;
		case MUL: regA=Pop(); regB=Pop(); regB*=regA; Push(regB); break;
		case DIV: regA=Pop(); regB=Pop(); regB/=regA; Push(regB); break;
		case PRINT: printf("%d\n",Pop()); break;
		case HLT: return;
		case LT: regA=Pop(); regB=Pop(); regA=regB<regA; Push(regA); break;
		case GT: regA=Pop(); regB=Pop(); regA=regB>regA; Push(regA); break;
		case PUSH_: Push(v); break;
		case SETVAR_: vars[v]=Pop(); break;
		case GETVAR_: Push(vars[v]); break;
		case JZ_: if (Pop()==1) comandCoounter=v; break;
		case JNZ_: if (Pop()==0) comandCoounter=v; break;
		case JMP_: comandCoounter=v; break;
		} 
		

	}
}