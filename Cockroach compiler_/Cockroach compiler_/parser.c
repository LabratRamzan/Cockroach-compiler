#include "parser.h"

int _position=0;

TreeNode* Parser(Token** toks)
{
	tokens=toks;
	return ParseStatements();
}

Token* _expectedToken(enum TokenType type)
{
	if ( type == tokens[_position]->Type )
		return tokens[_position];
	return 0;
}

Token* _acceptToken(enum TokenType type)
{
	Token* token = _expectedToken(type);
	if (token != 0) _position++;
	return token;
}

int _checkToken(enum TokenType type)
{
	Token* token = _expectedToken(type);
	if (token != 0)
	{
		_position++;
		return 1;
	}
	return 0;
}

int _setNcheck( TreeNode* to, TreeNode* from)
{
	if(from)
	{
		*to = *from;
		return 1;
	}
	to=0;
	return 0;
}

void set_next_nodes(TreeNode* node, TreeNode* br1,TreeNode* br2,TreeNode* br3)
{
	node->branch1=br1;
	node->branch2=br2;
	node->branch3=br3;
}

TreeNode* ParseIdentifier()
{
	TreeNode *identf;
	Token* ident = _acceptToken(Ident);
	if (ident==0) return 0;
	identf=(TreeNode*)malloc(sizeof(TreeNode));
	identf->nodeType=_identf;
	identf->token=ident;
	return identf;
}

TreeNode* ParseNumber()
{
	TreeNode *numb;
	Token* num = _acceptToken(Number);
	if (num) 
	{
		numb=(TreeNode*)malloc(sizeof(TreeNode));
		numb->nodeType=_number;
		numb->token=num;
		return numb;
	}
	return 0;
}

TreeNode* ParsePrint()
{
	TreeNode *expr, *print;
	print=(TreeNode*)malloc(sizeof(TreeNode));
	expr=(TreeNode*)malloc(sizeof(TreeNode));
	expr->nodeType=_expression;
	if (_checkToken(Print) && _checkToken(Lpar) &&
		_setNcheck(expr , ParseExpression()) && _checkToken(Rpar) ) 
	{
		print->nodeType=_print;
		set_next_nodes(print,expr,0,0);
		return print;
	}
	free(expr);
	free(print);
	return 0;
}

TreeNode* ParseAssigment()
{
	TreeNode *left_ident, *right_expr, *assig;
	left_ident=(TreeNode*)malloc(sizeof(TreeNode));
	right_expr=(TreeNode*)malloc(sizeof(TreeNode));
	left_ident->nodeType=_identf;
	right_expr->nodeType=_expression;
	if (_setNcheck(left_ident, ParseIdentifier()) && _checkToken(Assign) &&
		_setNcheck(right_expr, ParseExpression()) )
	{
		assig=(TreeNode*)malloc(sizeof(TreeNode));
		assig->nodeType=_assigment;
		assig->token=0;
		set_next_nodes(assig,left_ident,right_expr,0);
		return assig;
	}
	free(left_ident);
	free(right_expr);
	return 0;
	
}

TreeNode* ParseBreak()
{
	TreeNode* brek;

	if (_checkToken(Break))
	{
		brek=(TreeNode*)malloc(sizeof(TreeNode));
		brek->nodeType=_break;
		brek->token=0;
		return brek;
	}
	return 0;
}

TreeNode* ParseIfElse()
{
	TreeNode *iff, *cond, *ye, *no;
	int save_pos=_position;
	cond=(TreeNode*)malloc(sizeof(TreeNode));
	ye=(TreeNode*)malloc(sizeof(TreeNode));
	no=(TreeNode*)malloc(sizeof(TreeNode));
	cond->nodeType=_expression;
	if ( _checkToken(If) && _checkToken(Lpar) &&
		_setNcheck(cond, ParseExpression()) && _checkToken(Rpar) &&
		_setNcheck(ye, ParseStatement()) && _checkToken(Else) &&
		_setNcheck(no, ParseStatement()) )
	{
		iff=(TreeNode*)malloc(sizeof(TreeNode));
		iff->nodeType=_ifelse;
		set_next_nodes(iff,cond,ye,no);
		return iff;
	}
	_position=save_pos;
	free(cond);
	free(ye);
	free(no);
	return 0;
}

TreeNode* ParseIf()
{
	TreeNode *iff, *cond, *ye;
	cond=(TreeNode*)malloc(sizeof(TreeNode));
	ye=(TreeNode*)malloc(sizeof(TreeNode));
	cond->nodeType=_expression;
	if ( _checkToken(If) && _checkToken(Lpar) &&
		_setNcheck(cond, ParseExpression()) && _checkToken(Rpar) &&
		_setNcheck(ye, ParseStatement()) )
	{
		iff=(TreeNode*)malloc(sizeof(TreeNode));
		iff->nodeType=_if;
		set_next_nodes(iff,cond,ye,0);
		return iff;
	}
	free(cond);
	free(ye);
	return 0;
}

TreeNode* ParseWhile()
{
	TreeNode *whil, *cond, *body;
	cond=(TreeNode*)malloc(sizeof(TreeNode));
	body=(TreeNode*)malloc(sizeof(TreeNode));
	if  (_checkToken(While) && _checkToken(Lpar) &&
		_setNcheck(cond, ParseExpression()) && _checkToken(Rpar) &&
		_setNcheck(body, ParseStatement()) )
	{
		whil=(TreeNode*)malloc(sizeof(TreeNode));
		whil->nodeType=_while;
		set_next_nodes(whil, cond, body, 0);
		return whil;
	}
	free(cond);
	free(body);
	return 0;
}

TreeNode* ParseExpression()
{
	TreeNode *left, *right, *expr;
	left=ParseExpression0();
	right=(TreeNode*)malloc(sizeof(TreeNode));
	
	if (left==0) return 0;

	if (_checkToken(Lt) && _setNcheck(right,ParseExpression()) )
	{
		expr=(TreeNode*)malloc(sizeof(TreeNode));
		expr->nodeType=_lt;
		set_next_nodes(expr,left,right,0);
		return expr;
	}

	if (_checkToken(Leq) && _setNcheck(right,ParseExpression()) )
	{
		expr=(TreeNode*)malloc(sizeof(TreeNode));
		expr->nodeType=_leq;
		set_next_nodes(expr,left,right,0);
		return expr;
	}

	if (_checkToken(Gt) && _setNcheck(right,ParseExpression()) )
	{
		expr=(TreeNode*)malloc(sizeof(TreeNode));
		expr->nodeType=_gt;
		set_next_nodes(expr,left,right,0);
		return expr;
	}

	if (_checkToken(Geq) && _setNcheck(right,ParseExpression()) )
	{
		expr=(TreeNode*)malloc(sizeof(TreeNode));
		expr->nodeType=_geq;
		set_next_nodes(expr,left,right,0);
		return expr;
	}

	if (_checkToken(Eq) && _setNcheck(right,ParseExpression()) )
	{
		expr=(TreeNode*)malloc(sizeof(TreeNode));
		expr->nodeType=_equal;
		set_next_nodes(expr,left,right,0);
		return expr;
	}

	if (_checkToken(Neq) && _setNcheck(right,ParseExpression()) )
	{
		expr=(TreeNode*)malloc(sizeof(TreeNode));
		expr->nodeType=_nequal;
		set_next_nodes(expr,left,right,0);
		return expr;
	}
	free(right);
	return left;
}

TreeNode* ParseExpression0()
{
	TreeNode *left, *right, *op;
	left = ParseExpression1();
	right=(TreeNode*)malloc(sizeof(TreeNode));

	if (_checkToken(Plus) && _setNcheck(right,ParseExpression0()))
	{
		op=(TreeNode*)malloc(sizeof(TreeNode));
		op->nodeType=_plus;
		set_next_nodes(op,left,right,0);
		return op;
	}

	if (_checkToken(Minus) && _setNcheck(right,ParseExpression0()))
	{
		op=(TreeNode*)malloc(sizeof(TreeNode));
		op->nodeType=_minus;
		set_next_nodes(op,left,right,0);
		return op;
	}
	free(right);
	return left;
}

TreeNode* ParseExpression1()
{
	TreeNode *left,*right,*op;
	left=ParseAtom();
	right=(TreeNode*)malloc(sizeof(TreeNode));


	if (_checkToken(Mul) && _setNcheck(right,ParseExpression1()))
	{
		op=(TreeNode*)malloc(sizeof(TreeNode));
		op->nodeType=_mul;
		set_next_nodes(op,left,right,0);
		return op;
	}

	if (_checkToken(Div) && _setNcheck(right,ParseExpression1()))
	{
		op=(TreeNode*)malloc(sizeof(TreeNode));
		op->nodeType=_div;
		set_next_nodes(op,left,right,0);
		return op;
	}
	free(right);
	return left;
}

TreeNode* ParseAtom()
{
	TreeNode *inBr, *pr;
	inBr=(TreeNode*)malloc(sizeof(TreeNode));
	if( _checkToken(Lbrace) && _setNcheck(inBr,ParseExpression()) &&
		_checkToken(Rbrace) )
	{
		return inBr;
	}
	free(inBr);
	pr=ParseNumber();
	if (pr==0) return ParseIdentifier();
	else return pr;
}

TreeNode* ParseStatement()
{
	TreeNode* statement, *temp, *in;
	statement=(TreeNode*)malloc(sizeof(TreeNode));
	in=(TreeNode*)malloc(sizeof(TreeNode));
	temp=statement;
	if ( _checkToken(Lbrace) && _setNcheck(statement, ParseStatements()) && _checkToken(Rbrace) )
	{
		statement->nodeType=_block;
		statement->token=0;
		//set_next_nodes(statement,in,0,0);
		return statement;
	}
	statement = ParseAssigment(); if (statement) return statement;
	statement = ParseIfElse(); if (statement) return statement;
	statement = ParseIf(); if (statement) return statement;
	statement = ParseWhile(); if (statement) return statement;
	statement = ParsePrint(); if (statement) return statement;
	if (_checkToken(Break)) 
	{
		statement=temp;
		statement->nodeType=_break;
		return statement;
	}
	return 0;

}

TreeNode* ParseStatements()
{
	TreeNode *left, *right, *node;
	left=(TreeNode*)malloc(sizeof(TreeNode));
	right=(TreeNode*)malloc(sizeof(TreeNode));
	node=(TreeNode*)malloc(sizeof(TreeNode));
	
	node->nodeType=_node;
	node->token=0;
	
	left = ParseStatement();
	if (left == 0) return 0;
	if ( _checkToken(Semicolon) && _setNcheck(right, ParseStatements()) )
	{
		set_next_nodes(node,left,right,0);
		return node;
	}
	set_next_nodes(node,left,0,0);
	return node;

}




