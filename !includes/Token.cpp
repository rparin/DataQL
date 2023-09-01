#include "Token.h"

Token::Token()
{
	_token = "";
	_type = 0;
}

Token::Token(string str, int type)
{
	_token = str;
	_type = type;
}

int Token::type()
{
	return _type;
}

string Token::type_string()
{
		 if (_type == Alpha) return "Alpha";
	else if (_type == Num) return "Number";
	else if (_type == Punct) return "Punctuation";
	else if (_type == Space) return "Space";
	else if (_type == Dec) return "Decimal";
	else if (_type == Sym) return "Symbol";
	else if (_type == Op) return "Operator";
	else return "Unknown";
}

string Token::token_str()
{
	return _token;
}

ostream& operator<<(ostream& outs, const Token& t)
{
	outs << "|" << t._token << "|";
	return outs;
}

ostream& operator<<(ostream& outs, const Token* t)
{
	outs << t->_token;
	return outs;
}
