#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <stdlib.h>

enum TokenType 
{
		Semicolon,
        Plus,
        Minus,
        Mul,
        Div,
        Sub,
        If,
        Then,
        Else,
        Break,
        While,
        Lpar,
        Rpar,
        Lbrace,
        Rbrace,
        Lt,
        Leq,
        Gt,
        Geq,
        Eq,
		Neq,
        Assign,
        Print,
        Ident,
        Number,
        _EOF=-1,
};

typedef struct {
	char* Text;
	unsigned long From, To;
	enum TokenType Type;
} Token;

Token Token_CR( enum TokenType type, char* text, unsigned long from, unsigned long to);
#endif