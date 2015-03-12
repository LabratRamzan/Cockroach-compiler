#define _CRT_SECURE_NO_WARNINGS
#define VARMAX 256
#define COMMAX 256
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stack.h"

typedef struct{
	int comand;
	int arg;
}comand;

int getVarOrNum();

void SaveInctructions();

void Interpreter(void* a);

void Execution();