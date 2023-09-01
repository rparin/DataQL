#ifndef TOKEN_H
#define TOKEN_H

#include "../!includes/State_Machine.h"
#include <string>
#include <cassert>

class Token
{
public:
    //Constructors
    Token();
    Token(string str, int type);

    //Ostream Operater
    friend ostream& operator <<(ostream& outs, const Token& t);
    friend ostream& operator <<(ostream& outs, const Token* t);

    //Getters
    int type();             //What type it is in number
    string type_string();   //What type it is in String
    string token_str();     //Get the token as a String

private:
    string _token;
    int _type;
};

#endif //TOKEN_H
