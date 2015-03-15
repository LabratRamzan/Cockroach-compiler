#include "interpreter.h"

void Interpreter(void* a)
{
	Info_Interp* info = (Info_Interp*)malloc(sizeof(Info_Interp));
	info->instructions=a;
	SaveInctructions(info);
	Execution(info);
}

int getVarOrNum(Info_Interp* info)
{
	return info->comands[info->comandCoounter].arg;
}

void SaveInctructions(Info_Interp* info)
{
	unsigned long i=1;
	comand* com;
	com=(comand*)malloc(8);
	while(1)
	{
		com->comand=0;
		memcpy(&(com->comand),info->instructions,1);
		info->instructions=(char*)info->instructions+1;
		if (com->comand>8) 
		{
			memcpy(&(com->arg),info->instructions,4);
			info->instructions=(char*)info->instructions+4;
		}
		info->comands[i]=*com;
		i++;
		if(com->comand==HLT) break;
	}
}

void Execution(Info_Interp* info)
{
	double regA, regB;
	Stack* st =(Stack*)malloc(sizeof(Stack));
	int com=0;
	int v=0;
	st->size=0;
	info->comandCoounter=1;
	while (1)
	{
		com=info->comands[info->comandCoounter].comand;
		v=getVarOrNum(info);
		info->comandCoounter++;
		switch (com)
		{
		case POP: Pop(st);
		case ADD: regA=Pop(st); regB=Pop(st); regB+=regA; Push(regB,st); break;
		case SUB: regA=Pop(st); regB=Pop(st); regB-=regA; Push(regB,st); break;
		case MUL: regA=Pop(st); regB=Pop(st); regB*=regA; Push(regB,st); break;
		case DIV: regA=Pop(st); regB=Pop(st); regB/=regA; Push(regB,st); break;
		case PRINT: printf("%d\n",Pop(st)); break;
		case HLT: return;
		case LT: regA=Pop(st); regB=Pop(st); regA=regB<regA; Push(regA,st); break;
		case GT: regA=Pop(st); regB=Pop(st); regA=regB>regA; Push(regA,st); break;
		case PUSH_: Push(v,st); break;
		case SETVAR_: info->vars[v]=Pop(st); break;
		case GETVAR_: Push(info->vars[v],st); break;
		case JZ_: if (Pop(st)!=0) info->comandCoounter=v; break;
		case JNZ_: if (Pop(st)==0) info->comandCoounter=v; break;
		case JMP_: info->comandCoounter=v; break;
		} 
		

	}
}