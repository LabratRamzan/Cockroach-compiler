#include "token.h"

Token Token_CR( enum TokenType type, char* text, unsigned long from, unsigned long to)
{
	Token token;
	token.Type=type;
	token.Text=text;
	token.From=from;
	token.To=to;
	return token;
}