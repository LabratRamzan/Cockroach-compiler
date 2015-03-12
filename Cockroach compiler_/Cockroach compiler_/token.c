#include "token.h"

Token Token_CR( enum TokenType type, char* text, int from, int to)
{
	Token token;
	token.Type=type;
	token.Text=text;
	token.From=from;
	token.To=to;
	return token;
}