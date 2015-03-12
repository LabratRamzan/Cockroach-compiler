#ifndef _LEXER_H_
#define _LEXER_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <ctype.h>.
#include "token.h"
#include "linked_list.h"

	char* text;
	static int pos=0;

	void InitText(char* program);

	node* getTokens();

	int _expectSeparator(int offset);

	int _expectWord(char* str);

	int _eof();

	Token* _ident();

	Token* _next();

	Token** listToMassiv(node* nod);

	node* Lexer(char* program);

#endif