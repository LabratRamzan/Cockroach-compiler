#ifndef _TREE_H_
#define _TREE_H_

#include <stdlib.h>
#include "token.h"

enum treeNodeTypes
{
	_block,
	_if,
	_assigment,
	_ifelse,
	_while,
	_expression,
	_identf,
	_plus,
	_minus,
	_div,
	_mul,
	_lt,
    _leq,
    _gt,
    _geq,
	_equal,
	_nequal,
	_print,
	_break,
	_number,	
	_node
};

typedef struct treeNode{
	enum treeNodeTypes nodeType;
	Token* token;
	struct treeNode* branch1;
	struct treeNode* branch2;
	struct treeNode* branch3;
}TreeNode;

#endif