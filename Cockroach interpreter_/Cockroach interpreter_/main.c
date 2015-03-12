#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include "interpreter.h"

void* getBCode()
{
	FILE* fp;
	int size;
	void* code;
	char name[256];
	printf("Reading byte-code, file name: \n");
	scanf("%s",name);
	fp=fopen(name,"rb");
	fseek(fp,0,SEEK_END);
	size=ftell(fp);
	fseek(fp,0,0);
	code=malloc(size);
	fread(code,1,size,fp);
	return code;
}

main()
{
	void* code=getBCode();
	Interpreter(code);
}