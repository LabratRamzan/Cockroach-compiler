#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdlib.h>
#include "lexer.h"
#include "token.h"
#include "tree.h"

Token** tokens;

TreeNode* Parser(Token** toks);

Token* _expectedToken(enum TokenType type);

Token* _acceptToken(enum TokenType type);

int _checkToken(enum TokenType type);

int _setNcheck( TreeNode to, TreeNode from);

void set_next_nodes(TreeNode* node, int cnt, TreeNode* nodes,...);

TreeNode* ParsePrint();

TreeNode* ParseIdentifier();

TreeNode* ParseAssigment();

TreeNode* ParseIf();

TreeNode* ParseWhile();

TreeNode* ParseNumber();

TreeNode* ParseExpression();

TreeNode* ParseExpression0();

TreeNode* ParseExpression1();

TreeNode* ParseAtom();

TreeNode* ParseBreak();

TreeNode* ParseStatement();

TreeNode* ParseStatements();

#endif