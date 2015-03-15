#include "parser.h"


TreeNode* Parser(Token** toks)
{
	TreeNode* node;
	Info_Parser* info = (Info_Parser*)malloc(sizeof(Info_Parser));
	info->tokens=toks;
	info->pos=0;
	node= ParseStatements(info);
	free(info);
	return node;
}

Token* _expectedToken(enum TokenType type, Info_Parser* info)
{
	if ( type == info->tokens[info->pos]->Type )
		return info->tokens[info->pos];
	return 0;
}

Token* _acceptToken(enum TokenType type, Info_Parser* info)
{
	Token* token = _expectedToken(type,info);
	if (token != 0) info->pos++;
	return token;
}

int _checkToken(enum TokenType type, Info_Parser* info)
{
	Token* token = _expectedToken(type, info);
	if (token != 0)
	{
		info->pos++;
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

TreeNode* ParseIdentifier(Info_Parser* info)
{
	TreeNode *identf;
	Token* ident = _acceptToken(Ident, info);
	if (ident==0) return 0;
	identf=(TreeNode*)malloc(sizeof(TreeNode));
	identf->nodeType=_identf;
	identf->token=ident;
	return identf;
}

TreeNode* ParseNumber(Info_Parser* info)
{
	TreeNode *numb;
	Token* num = _acceptToken(Number,info);
	if (num) 
	{
		numb=(TreeNode*)malloc(sizeof(TreeNode));
		numb->nodeType=_number;
		numb->token=num;
		return numb;
	}
	return 0;
}

TreeNode* ParsePrint(Info_Parser* info)
{
	TreeNode *expr, *print;
	print=(TreeNode*)malloc(sizeof(TreeNode));
	expr=(TreeNode*)malloc(sizeof(TreeNode));
	expr->nodeType=_expression;
	if (_checkToken(Print, info) && _checkToken(Lpar, info) &&
		_setNcheck(expr , ParseExpression(info)) && _checkToken(Rpar, info) ) 
	{
		print->nodeType=_print;
		set_next_nodes(print,expr,0,0);
		return print;
	}
	free(expr);
	free(print);
	return 0;
}

TreeNode* ParseAssigment(Info_Parser* info)
{
	TreeNode *left_ident, *right_expr, *assig;
	left_ident=(TreeNode*)malloc(sizeof(TreeNode));
	right_expr=(TreeNode*)malloc(sizeof(TreeNode));
	left_ident->nodeType=_identf;
	right_expr->nodeType=_expression;
	if (_setNcheck(left_ident, ParseIdentifier(info)) && _checkToken(Assign,info) &&
		_setNcheck(right_expr, ParseExpression(info)) )
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

TreeNode* ParseBreak(Info_Parser* info)
{
	TreeNode* brek;

	if (_checkToken(Break, info))
	{
		brek=(TreeNode*)malloc(sizeof(TreeNode));
		brek->nodeType=_break;
		brek->token=0;
		return brek;
	}
	return 0;
}

TreeNode* ParseIfElse(Info_Parser* info)
{
	TreeNode *iff, *cond, *ye, *no;
	unsigned long save_pos=info->pos;
	cond=(TreeNode*)malloc(sizeof(TreeNode));
	ye=(TreeNode*)malloc(sizeof(TreeNode));
	no=(TreeNode*)malloc(sizeof(TreeNode));
	cond->nodeType=_expression;
	if ( _checkToken(If,info) && _checkToken(Lpar,info) &&
		_setNcheck(cond, ParseExpression(info)) && _checkToken(Rpar,info) &&
		_setNcheck(ye, ParseStatement(info)) && _checkToken(Else,info) &&
		_setNcheck(no, ParseStatement(info)) )
	{
		iff=(TreeNode*)malloc(sizeof(TreeNode));
		iff->nodeType=_ifelse;
		set_next_nodes(iff,cond,ye,no);
		return iff;
	}
	info->pos=save_pos;
	free(cond);
	free(ye);
	free(no);
	return 0;
}

TreeNode* ParseIf(Info_Parser* info)
{
	TreeNode *iff, *cond, *ye;
	cond=(TreeNode*)malloc(sizeof(TreeNode));
	ye=(TreeNode*)malloc(sizeof(TreeNode));
	cond->nodeType=_expression;
	if ( _checkToken(If,info) && _checkToken(Lpar,info) &&
		_setNcheck(cond, ParseExpression(info)) && _checkToken(Rpar,info) &&
		_setNcheck(ye, ParseStatement(info)) )
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

TreeNode* ParseWhile(Info_Parser* info)
{
	TreeNode *whil, *cond, *body;
	cond=(TreeNode*)malloc(sizeof(TreeNode));
	body=(TreeNode*)malloc(sizeof(TreeNode));
	if  (_checkToken(While,info) && _checkToken(Lpar,info) &&
		_setNcheck(cond, ParseExpression(info)) && _checkToken(Rpar,info) &&
		_setNcheck(body, ParseStatement(info)) )
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

TreeNode* ParseExpression(Info_Parser* info)
{
	TreeNode *left, *right, *expr;
	left=ParseExpression0(info);
	right=(TreeNode*)malloc(sizeof(TreeNode));
	
	if (left==0) return 0;

	if (_checkToken(Lt,info) && _setNcheck(right,ParseExpression(info)) )
	{
		expr=(TreeNode*)malloc(sizeof(TreeNode));
		expr->nodeType=_lt;
		set_next_nodes(expr,left,right,0);
		return expr;
	}

	if (_checkToken(Leq,info) && _setNcheck(right,ParseExpression(info)) )
	{
		expr=(TreeNode*)malloc(sizeof(TreeNode));
		expr->nodeType=_leq;
		set_next_nodes(expr,left,right,0);
		return expr;
	}

	if (_checkToken(Gt,info) && _setNcheck(right,ParseExpression(info)) )
	{
		expr=(TreeNode*)malloc(sizeof(TreeNode));
		expr->nodeType=_gt;
		set_next_nodes(expr,left,right,0);
		return expr;
	}

	if (_checkToken(Geq,info) && _setNcheck(right,ParseExpression(info)) )
	{
		expr=(TreeNode*)malloc(sizeof(TreeNode));
		expr->nodeType=_geq;
		set_next_nodes(expr,left,right,0);
		return expr;
	}

	if (_checkToken(Eq,info) && _setNcheck(right,ParseExpression(info)) )
	{
		expr=(TreeNode*)malloc(sizeof(TreeNode));
		expr->nodeType=_equal;
		set_next_nodes(expr,left,right,0);
		return expr;
	}

	if (_checkToken(Neq,info) && _setNcheck(right,ParseExpression(info)) )
	{
		expr=(TreeNode*)malloc(sizeof(TreeNode));
		expr->nodeType=_nequal;
		set_next_nodes(expr,left,right,0);
		return expr;
	}
	free(right);
	return left;
}

TreeNode* ParseExpression0(Info_Parser* info)
{
	TreeNode *left, *right, *op;
	left = ParseExpression1(info);
	right=(TreeNode*)malloc(sizeof(TreeNode));

	if (_checkToken(Plus,info) && _setNcheck(right,ParseExpression0(info)))
	{
		op=(TreeNode*)malloc(sizeof(TreeNode));
		op->nodeType=_plus;
		set_next_nodes(op,left,right,0);
		return op;
	}

	if (_checkToken(Minus,info) && _setNcheck(right,ParseExpression0(info)))
	{
		op=(TreeNode*)malloc(sizeof(TreeNode));
		op->nodeType=_minus;
		set_next_nodes(op,left,right,0);
		return op;
	}
	free(right);
	return left;
}

TreeNode* ParseExpression1(Info_Parser* info)
{
	TreeNode *left,*right,*op;
	left=ParseAtom(info);
	right=(TreeNode*)malloc(sizeof(TreeNode));


	if (_checkToken(Mul,info) && _setNcheck(right,ParseExpression1(info)))
	{
		op=(TreeNode*)malloc(sizeof(TreeNode));
		op->nodeType=_mul;
		set_next_nodes(op,left,right,0);
		return op;
	}

	if (_checkToken(Div,info) && _setNcheck(right,ParseExpression1(info)))
	{
		op=(TreeNode*)malloc(sizeof(TreeNode));
		op->nodeType=_div;
		set_next_nodes(op,left,right,0);
		return op;
	}
	free(right);
	return left;
}

TreeNode* ParseAtom(Info_Parser* info)
{
	TreeNode *inBr, *pr;
	inBr=(TreeNode*)malloc(sizeof(TreeNode));
	if( _checkToken(Lbrace,info) && _setNcheck(inBr,ParseExpression(info)) &&
		_checkToken(Rbrace,info) )
	{
		return inBr;
	}
	free(inBr);
	pr=ParseNumber(info);
	if (pr==0) return ParseIdentifier(info);
	else return pr;
}

TreeNode* ParseStatement(Info_Parser* info)
{
	TreeNode* statement, *temp, *in;
	statement=(TreeNode*)malloc(sizeof(TreeNode));
	in=(TreeNode*)malloc(sizeof(TreeNode));
	temp=statement;
	if ( _checkToken(Lbrace,info) && _setNcheck(statement, ParseStatements(info)) && _checkToken(Rbrace,info) )
	{
		statement->nodeType=_block;
		statement->token=0;
		//set_next_nodes(statement,in,0,0);
		return statement;
	}
	statement = ParseAssigment(info); if (statement) return statement;
	statement = ParseIfElse(info); if (statement) return statement;
	statement = ParseIf(info); if (statement) return statement;
	statement = ParseWhile(info); if (statement) return statement;
	statement = ParsePrint(info); if (statement) return statement;
	if (_checkToken(Break,info)) 
	{
		statement=temp;
		statement->nodeType=_break;
		return statement;
	}
	return 0;

}

TreeNode* ParseStatements(Info_Parser* info)
{
	TreeNode *left, *right, *node;
	left=(TreeNode*)malloc(sizeof(TreeNode));
	right=(TreeNode*)malloc(sizeof(TreeNode));
	node=(TreeNode*)malloc(sizeof(TreeNode));
	
	node->nodeType=_node;
	node->token=0;
	
	left = ParseStatement(info);
	if (left == 0) return 0;
	if ( _checkToken(Semicolon,info) && _setNcheck(right, ParseStatements(info)) )
	{
		set_next_nodes(node,left,right,0);
		return node;
	}
	set_next_nodes(node,left,0,0);
	return node;

}




