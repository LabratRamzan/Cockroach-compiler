#include "lexer.h"

void InitText(char* program, Info_Lexer* info)
	{
		unsigned long len = strlen(program);
		info->text=strcat( strcpy((char*)malloc(sizeof(char)*(len+1)) , program) , "$");
	}

	node* getTokens(Info_Lexer* info)
    {
		Token* token;
        node* nod;
		nod = (node *)malloc(sizeof(node));
		token = _next(info);
		if (token==0) return 0;
		nod->data=*token;
		nod->next=0;
        for ( token = _next(info) ; token != 0; token = _next(info) )
			add_token(nod, *token);
		return nod;
    }

	int _expectSeparator(unsigned long offset, Info_Lexer* info)
	{
		if ( isalnum(info->text[info->pos+offset]) || info->text[info->pos+offset]=='_') return 0;
		return 1;
	}

	int _expectWord(char* str,Info_Lexer* info)
	{
		if ( !(strncmp(info->text+info->pos,str,strlen(str))) && _expectSeparator(strlen(str),info) ) return 1;
		return 0;
	}

	int _eof(Info_Lexer* info)
	{
		return info->pos>=strlen(info->text);
	}

	Token* _ident(Info_Lexer* info)
	{
		unsigned long end=info->pos;
		if ( isalpha(info->text[end]) || info->text[end]=='_' ) end++;
		while ( isalnum(info->text[end]) || info->text[end] == '_') end++;
		if (end != info->pos)
		{
			Token result = Token_CR(Ident, info->text, info->pos, end);
			info->pos = end;
			return &result;
		}
		else return 0;
	}

	Token* _keyword( enum TokenType type, unsigned long size, Info_Lexer* info)
	{
		Token result = Token_CR(type, info->text, info->pos, info->pos + size);
		info->pos = info->pos + size;
		return &result;
	}

	Token* _number(Info_Lexer* info)
	{
		Token result;
		unsigned long start = info->pos;
		while ( isdigit(info->text[info->pos]) ) info->pos++;
		result = Token_CR(Number, info->text, start, info->pos);
		if ( info->pos != start ) return &result;
		else return 0;
	}

	Token* _next(Info_Lexer* info)
	{
		Token* ident;
		Token* num;

		if (_eof(info)) return 0;
		while ( (info->pos < strlen(info->text)) && isspace(info->text[info->pos]) ) info->pos++;
		if (_eof(info)) return 0;
			
		switch (info->text[info->pos])
		{
			case '$': return _keyword(_EOF, 1,info);
            case ';': return _keyword(Semicolon, 1, info);
			case '*': return _keyword(Mul, 1, info);
            case '/': return _keyword(Div, 1, info);
            case '-': return _keyword(Minus, 1, info);
            case '+': return _keyword(Plus, 1, info);
            case '(': return _keyword(Lpar, 1, info);
            case ')': return _keyword(Rpar, 1, info);
            case '{': return _keyword(Lbrace, 1, info);
            case '}': return _keyword(Rbrace, 1, info);
            case '<': switch (info->text[info->pos + 1])
							{
                    case '=': return _keyword(Leq, 2, info);
                    default: return _keyword(Lt, 1, info); 
                }
            case '>': switch (info->text[info->pos + 1])
                {
                    case '=': return _keyword(Geq, 2, info);
                    default: return _keyword(Gt, 1, info);
                }
            case '=': switch (info->text[info->pos + 1])
                {
                    case '=': return _keyword(Eq, 2, info);
                    default: return _keyword(Assign, 1, info); 
                }
			case '!' : if (info->text[info->pos + 1]=='=') return _keyword(Neq,2, info); return 0;
            default:
                if (_expectWord("if", info))
                    return _keyword(If, 2, info);
                if (_expectWord("else", info))
                    return _keyword(Else, 4, info);
                if (_expectWord("break", info))
                    return _keyword(Break, 5, info);
                if (_expectWord("while", info))
                    return _keyword(While, 5, info);
                if (_expectWord("print", info))
                    return _keyword(Print, 5, info);
                num = _number(info);
				if (num != 0) return num;
                ident = _ident(info);
				if (ident != 0) return ident;
                return 0;
		}
	}

	Token** listToMassiv(node* nod)
{
	unsigned long i;
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

	void _setTokenName(Token** mas,Info_Lexer* info)
	{
		unsigned long len;
		while (1)
		{
			len = (*mas)->To - (*mas)->From;
			printf("%s | %d %d",info->text,(*mas)->From,(*mas)->To);
			(*mas)->Text = (char*)malloc(len+1);
			printf("%s | %d\n",(*mas)->Text, (*mas)->Text);
			//memcpy((*mas)->Text, info->text+(*mas)->From,len );
			for(len=0; (len+(*mas)->From) < (*mas)->To ;len++)
				(*mas)->Text[len]=info->text[(*mas)->From+len];
			printf("%s | %d\n",(*mas)->Text, (*mas)->Text);
			(*mas)->Text[len]='\0';
			printf("%s | %d\n",(*mas)->Text, (*mas)->Text);
			if ( (*mas)->Type == _EOF ) break; 
			mas++;
		}
	}

	node* Lexer(char* program)
	{
		Token** mass;
		node* tokList;
		Info_Lexer *info = (Info_Lexer*)malloc(sizeof(Info_Lexer));
		info->pos=0;
		InitText(program,info);
		tokList = getTokens(info);
		mass = listToMassiv(tokList);
		_setTokenName(mass, info);
		free(info);
		return tokList;
	}