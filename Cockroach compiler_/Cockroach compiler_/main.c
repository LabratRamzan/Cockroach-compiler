#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include "token.h"
#include "linked_list.h"
#include "lexer.h"
#include "parser.h"
#include "tree.h"
#include "compiler_test.h"
#include "compiler.h"

void printTokens(Token** tokens)
{
	int i=0;
	while (1)
	{
		printf("%d : %s,\t\t %d, %d\n",i,(*tokens)->Text,(*tokens)->From,(*tokens)->To);
		if ( (*tokens)->Type == _EOF ) break; 
		tokens++; i++;
	}
}

char* getProgram()
{
	FILE* fp1;
	int size;
	char* program;
	char code[256];
	printf("Reading program, file name: \n");
	scanf("%s",code);
	fp1=fopen(code,"rb");
	fseek(fp1,0,SEEK_END);
	size=ftell(fp1);
	fseek(fp1,0,0);
	program=(char*)malloc(size+1);
	fread(program,1,size,fp1);
	fclose(fp1);
	program[size]='\0';

	return program;
}

void saveBCode(void* c,int offset)
{
	FILE* fp;
	char code[256];

	printf("Record bytecode, file name: \n");
	scanf("%s",code);

	fp=fopen(code, "wb");
	fwrite(c,1,offset,fp);
	fclose(fp);
}

main()
{
	node *TokenList;
	Token** TokenMassiv;
	TreeNode* tree;
	void* bc;
    char* program;
	unsigned long offset=0;
	
	program=getProgram();

	TokenList = Lexer(program);
	TokenMassiv = listToMassiv(TokenList);
	printTokens(TokenMassiv);
	
	tree = Parser(TokenMassiv);

	TComp_statements(tree);
	bc=Compiler(tree, &offset);
	
	saveBCode(bc,offset);
	system("pause");
}