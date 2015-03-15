#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdlib.h>
#include "lexer.h"
#include "token.h"
#include "tree.h"



typedef struct
{
	Token** tokens;
	unsigned long pos;
}Info_Parser;

TreeNode* Parser(Token** toks);

Token* _expectedToken(enum TokenType type, Info_Parser* info);

Token* _acceptToken(enum TokenType type, Info_Parser* info);

int _checkToken(enum TokenType type, Info_Parser* info);

int _setNcheck( TreeNode to, TreeNode from, Info_Parser* info);

void set_next_nodes(TreeNode* node, TreeNode* br1,TreeNode* br2,TreeNode* br3);

TreeNode* ParsePrint(Info_Parser* info);

TreeNode* ParseIdentifier(Info_Parser* info);

TreeNode* ParseAssigment(Info_Parser* info);

TreeNode* ParseIf(Info_Parser* info);

TreeNode* ParseWhile(Info_Parser* info);

TreeNode* ParseNumber(Info_Parser* info);

TreeNode* ParseExpression(Info_Parser* info);

TreeNode* ParseExpression0(Info_Parser* info);

TreeNode* ParseExpression1(Info_Parser* info);

TreeNode* ParseAtom(Info_Parser* info);

TreeNode* ParseBreak(Info_Parser* info);

TreeNode* ParseStatement(Info_Parser* info);

TreeNode* ParseStatements(Info_Parser* info);

#endif