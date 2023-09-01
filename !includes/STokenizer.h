#ifndef STOKENIZER_H
#define STOKENIZER_H

#include "Token.h"
#include <string>

class STokenizer
{
public:
	//Constructors
	STokenizer();
    STokenizer(char str[]);
    STokenizer(string str);

    bool done();            //true: there are no more tokens
    bool more();            //true: there are more tokens

    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator >> (STokenizer& s, Token& t);

    //set a new string as the input string
    void set_string(const char* str);

private:
    static const int MAX_BUFFER = 200;

    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table();

    //extract the longest string that match
    //     one of the acceptable token types
    bool get_token(int& end_state, string& token);

    char _buffer[MAX_BUFFER+1];     //input string (+1 for NULL)
    int _len;						//Length of string
    int _pos;                       //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];
};

#endif //STOKENIZER_H