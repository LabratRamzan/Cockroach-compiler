#include "lexer.h"

	void InitText(char* program)
	{
		int len = strlen(program);
		text=strcat( strcpy((char*)malloc(sizeof(char)*(len+1)) , program) , "$");
	}

	node* getTokens()
    {
		Token* token;
        node* nod;
		nod = (node *)malloc(sizeof(node));
		token = _next();
		if (token==0) return 0;
		nod->data=*token;
		nod->next=0;
        for ( token = _next() ; token != 0; token = _next() )
			add_token(nod, *token);
		return nod;
    }

	int _expectSeparator(int offset)
	{
		if ( isalnum(text[pos+offset]) || text[pos+offset]=='_') return 0;
		return 1;
	}

	int _expectWord(char* str)
	{
		if ( !(strncmp(text+pos,str,strlen(str))) && _expectSeparator(strlen(str)) ) return 1;
		return 0;
	}

	int _eof()
	{
		return pos>=strlen(text);
	}

	Token* _ident()
	{
		int end=pos;
		if ( isalpha(text[end]) || text[end]=='_' ) end++;
		while ( isalnum(text[end]) || text[end] == '_') end++;
		if (end != pos)
		{
			Token result = Token_CR(Ident, text, pos, end);
			pos = end;
			return &result;
		}
		else return 0;
	}

	Token* _keyword( enum TokenType type, int size)
	{
		Token result = Token_CR(type, text, pos, pos + size);
		pos = pos + size;
		return &result;
	}

	Token* _number()
	{
		Token result;
		int start = pos;
		while ( isdigit(text[pos]) ) pos++;
		result = Token_CR(Number, text, start, pos);
		if ( pos != start ) return &result;
		else return 0;
	}

	Token* _next()
	{
		Token* ident;
		Token* num;

		if (_eof()) return 0;
		while ( (pos < strlen(text)) && isspace(text[pos]) ) pos++;
		if (_eof()) return 0;
			
		switch (text[pos])
		{
			case '$': return _keyword(_EOF, 1);
            case ';': return _keyword(Semicolon, 1);
			case '*': return _keyword(Mul, 1);
            case '/': return _keyword(Div, 1);
            case '-': return _keyword(Minus, 1);
            case '+': return _keyword(Plus, 1);
            case '(': return _keyword(Lpar, 1);
            case ')': return _keyword(Rpar, 1);
            case '{': return _keyword(Lbrace, 1);
            case '}': return _keyword(Rbrace, 1);
            case '<': switch (text[pos + 1])
							{
                    case '=': return _keyword(Leq, 2);
                    default: return _keyword(Lt, 1); 
                }
            case '>': switch (text[pos + 1])
                {
                    case '=': return _keyword(Geq, 2);
                    default: return _keyword(Gt, 1);
                }
            case '=': switch (text[pos + 1])
                {
                    case '=': return _keyword(Eq, 2);
                    default: return _keyword(Assign, 1); 
                }
			case '!' : if (text[pos + 1]=='=') return _keyword(Neq,2); return 0;
            default:
                if (_expectWord("if"))
                    return _keyword(If, 2);
                if (_expectWord("else"))
                    return _keyword(Else, 4);
                if (_expectWord("break"))
                    return _keyword(Break, 5);
                if (_expectWord("while"))
                    return _keyword(While, 5);
                if (_expectWord("print"))
                    return _keyword(Print, 5);
                num = _number();
				if (num != 0) return num;
                ident = _ident();
				if (ident != 0) return ident;
                return 0;
		}
	}

	Token** listToMassiv(node* nod)
{
	int cnt,i;
	Token** tok;
	node* tmp=nod;
	i=0;
	tok = (Token**)malloc(sizeof(Token*)*list_len(nod));
	while ( tmp!=0 )
	{
		tok[i]=&(tmp->data);
		i++;
		tmp=tmp->next;
	}
	return tok;
}

	void _setTokenName(Token** mas)
	{
		int len;
		while (1)
		{
			len = (*mas)->To - (*mas)->From;
			(*mas)->Text = (char*)malloc( sizeof(char)*(len+1));
			memcpy((*mas)->Text, &text[(*mas)->From],len );
			(*mas)->Text[len]='\0';
			if ( (*mas)->Type == _EOF ) break; 
			mas++;
		}
	}

	node* Lexer(char* program)
	{
		node* tokList;
		Token** tokMas;
		InitText(program);
		tokList = getTokens();
		tokMas = listToMassiv(tokList);
		_setTokenName(tokMas);
		return tokList;
	}