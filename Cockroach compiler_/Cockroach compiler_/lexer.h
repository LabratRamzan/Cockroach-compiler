#ifndef _LEXER_H_
#define _LEXER_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <ctype.h>.
#include "token.h"
#include "linked_list.h"


typedef struct{
	char* text;
	unsigned long pos;
}Info_Lexer;

	void InitText(char* program, Info_Lexer* info);

	node* getTokens(Info_Lexer* info);

	int _expectSeparator(unsigned long offset, Info_Lexer* info);

	int _expectWord(char* str, Info_Lexer* info);

	int _eof(Info_Lexer* info);

	Token* _ident(Info_Lexer* info);

	Token* _next(Info_Lexer* info);

	Token** listToMassiv(node* nod);

	node* Lexer(char* program);

#endif