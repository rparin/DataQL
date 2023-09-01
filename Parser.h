#ifndef PARSER_H
#define PARSER_H
#include "../!includes/STokenizer.h"
#include "../!includes/MMap.h"
#include "../!includes/Map.h"


class Parser {
public:

    //Constructors
    Parser(char str[]);
    Parser(string str = "");

    //Returns vector of strings
    Vector<Token> get_vectoks();

    //Parser Tokenizer
    bool make_parse_tree();

    //Tokenize new string
    void set_string(char str[]);
    void set_string(string str);

    //Parse string input to tree of commands and fields
    MMap<string, string> get_ptree();

    //Create Keywords MMap
    void set_keywords();

    //Returns if parsed input is valid
    bool is_valid();

private:
    static const int MAX_BUFFER = 200;

    //Keywords and States
    enum keyTypes : int {
        //States
        Fail_State = -1, Success_State = 1, Start_State = 0,

        //Select Machine
        Select = 5, Star, From, Table_Name,

        Fields = 10, Select_Comma, Where,

        //Insert Into Machine
        Insert = 15, Insert_Comma, Values,

        //Parenthesis
        LPar = 20, RPar,

        //General
        Command = 30, Symbol, Quote, Quote_C, Post_Table
    };

    //Parses string or cstring to vector of strings
    Vector<Token> parse(string str);

    //State Machine Table
    void make_table();

    //Formates ptree
    void format_tree();

    //Uppercase letters in string after space
    void formatstr(string& str);

    //Uppercase strings in vector
    void formatvec(Vector<string>& list);

    //lowercase string
    string lowerstr(string str);

    //Converts condition str to vector string
    void parse_condition();

    //Prints State and string during state machine
    void parse_debug(string state, string tok_str);

    Vector<Token> _tokens;
    MMap<string,string> _ptree;
    Map<string, int> _keywords;
    bool _valid_ptree;
    bool _debug;
    static int _ptable[MAX_ROWS][MAX_COLUMNS];
};

#endif //PARSER_H
