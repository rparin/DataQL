#include "Parser.h"
int Parser::_ptable[MAX_ROWS][MAX_COLUMNS];

Parser::Parser(char str[]):
	_tokens(), _keywords(), 
    _valid_ptree(false), _debug(true) {
    _tokens = parse(str);
    set_keywords();
    make_table();
    make_parse_tree();
    format_tree();
}

Parser::Parser(string str) :
	_tokens(), _keywords(),
    _valid_ptree(false), _debug(false) {
    _tokens = parse(str);
    set_keywords();
    make_table();
    make_parse_tree();
    format_tree();
}

void Parser::set_string(char str[]) {
    *this = Parser(str);
}

void Parser::set_string(string str){
    *this = Parser(str);
}

Vector<Token> Parser::get_vectoks() {
    return _tokens;
}

MMap<string, string> Parser::get_ptree() {
    return _ptree;
}

void Parser::set_keywords() {
    _keywords.insert("select", Select);
    _keywords.insert("*", Star);
    _keywords.insert("delete", From);
    _keywords.insert("from", From);
    _keywords.insert("table", Command);
    _keywords.insert("insert", Command);
    _keywords.insert("into", Command);
    _keywords.insert("drop", Command);
    _keywords.insert("create", Command);
    _keywords.insert("make", Command);
    _keywords.insert("where", Where);
    _keywords.insert("values", Values);
    _keywords.insert("fields", Fields);
    _keywords.insert("load", Table_Name);
    _keywords.insert("\"", Quote);
    _keywords.insert("\'", Quote);
    _keywords.insert("<", Symbol);
    _keywords.insert(">", Symbol);
    _keywords.insert(">=", Symbol);
    _keywords.insert("<=", Symbol);
    _keywords.insert("=", Symbol);
    _keywords.insert("and", Symbol);
    _keywords.insert("or", Symbol);
    _keywords.insert("(", LPar);
    _keywords.insert(")", RPar);
}

bool Parser::is_valid() {
    return _valid_ptree;
}

Vector<Token> Parser::parse(string str) {
    Vector<Token> tokens;
    STokenizer stk(str);
    Token t; string prev = "";

    //Process Tokens
    stk >> t;
    while (stk.more()) {

        //Ignore Spaces and tabs
        if (t.type() != Space && t.type() != Unknown) {

            if (!tokens.empty())
                prev = tokens[tokens.size() - 1].token_str();

            //Process <= or >=
            if (t.token_str() == "=" && (prev == "<" || prev == ">")) {
                tokens[tokens.size() - 1] = Token(prev + "=", Op);
            }

            //Add token to vector of tokens
            else tokens.push_back(t);
        }

        //Process next token
        t = Token();
        stk >> t;
    }

    return tokens;
}

void Parser::make_table() {

    //Initialize table to -1
    init_table(_ptable);

    //Setting which states are valid
    mark_success(_ptable, Table_Name);
    mark_success(_ptable, Values);
    mark_success(_ptable, Fields);
    mark_success(_ptable, Quote);
    mark_success(_ptable, Where);
    mark_success(_ptable, Quote_C);

    //Select Machine -Star
    mark_cell(Start_State, _ptable, Select, Select);
    mark_cell(Select, _ptable, From, From);
    mark_cell(From, _ptable, Table_Name, Table_Name);
    mark_cell(Table_Name, _ptable, Table_Name, Table_Name);

    //Select Machine -Fields
    mark_cell(Select, _ptable, Select_Comma, Select_Comma);
    mark_cell(Select_Comma, _ptable, Fields, Fields);
    mark_cell(Fields, _ptable, Select_Comma, Select_Comma);
    mark_cell(Select_Comma, _ptable, Table_Name, Table_Name);
    mark_cell(Post_Table, _ptable, Fields, Fields);
    mark_cell(Select, _ptable, Quote, Quote);
    mark_cell(Quote, _ptable, Select_Comma, Select_Comma);

    //Create Table Machine
    mark_cell(Start_State, _ptable, Command, Command);
    mark_cell(Command, _ptable, Table_Name, Table_Name);
    mark_cell(Table_Name, _ptable, Insert_Comma, Insert_Comma);

    //Insert Into Machine
    mark_cell(Table_Name, _ptable, Post_Table, Post_Table);
    mark_cell(Post_Table, _ptable, Values, Values);
    mark_cell(Insert_Comma, _ptable, Values, Values);
    mark_cell(Values, _ptable, Insert_Comma, Insert_Comma);

    //Quote Machine
    mark_cell(Quote, _ptable, Quote, Quote);
    mark_cell(Quote, _ptable, Insert_Comma, Insert_Comma);
    mark_cell(Insert_Comma, _ptable, Values, Values);
    mark_cell(Values, _ptable, Quote, Quote);

    //Load Machine
    mark_cell(Start_State, _ptable, Table_Name, Table_Name);

    //Delete Where Machine
    mark_cell(Start_State, _ptable, From, From);

    //Where Machine
    mark_cell(Post_Table, _ptable, Where, Where);
    mark_cell(Where, _ptable, Symbol, Symbol);
    mark_cell(Symbol, _ptable, Where, Where);
    mark_cell(Where, _ptable, Quote_C, Quote_C);
    mark_cell(Quote_C, _ptable, Quote_C, Quote_C);
    mark_cell(Quote_C, _ptable, Symbol, Symbol);
    mark_cell(Where, _ptable, Where, Where);
    mark_cell(Symbol, _ptable, Symbol, Symbol);
}

void Parser::formatstr(string& str){
    string format = str;
    bool space = false;
    for (int i = 0; i < str.size(); i++) {
        if (i == 0 || space) {
            format[i] = toupper(format[i]);
            space = false;
        }
        else format[i] = tolower(format[i]);
        if (format[i] == ' ') space = true;
        if (format[i] == '\"') space = true;
        if (format[i] == '\'') space = true;
    }
    str = format;
}

void Parser::formatvec(Vector<string>& list) {
    for (int i = 0; i < list.size(); i++) {
        formatstr(list[i]);
    }
}

string Parser::lowerstr(string str) {
    string lower = "";
    for (int i = 0; i < str.size(); i++) {
        lower += tolower(str[i]);
    }
	return lower;
}

void Parser::parse_condition() {
    Vector<string> where;
    string cond = _ptree["where"][0];
    Vector<Token> conditions = parse(cond);
    string word = ""; bool quote = false;

    for (int i = 0; i < conditions.size(); i++) {

        //Add Quoted Word
        if (conditions[i].type() == Punct) {
            if (quote) {
                where += word;
                quote = false;
                word = "";
            }
            else quote = true;
        }

        //Grab quoted word
        else if (quote) {
            if (word != "") word += " ";
            word += conditions[i].token_str();
        }

        //Add value/field to vector<string>
        else where += conditions[i].token_str();
    }

    //Update list of conditions
    _ptree["where"] = where;
}

void Parser::parse_debug(string state, string tok_str) {
    if (_debug) {
        cout << "State:\t" << left << setw(14) << state <<
            "Token:" << right << setw(3) 
            << "|" << tok_str << "|" << endl;
    }
}

bool Parser::make_parse_tree(){
    int nextState = 0, curState = 0;
    int cond_num = 0, lp = 0, rp = 0;
    string tok_str, ltok_str;
    string word = "",  condition = "";
    bool field = false;

    //Go Through Tokens
    for (int i = 0; i < _tokens.size(); i++) {
        tok_str = _tokens[i].token_str();
        ltok_str = lowerstr(tok_str);
        curState = nextState;
        
		switch (nextState) {
		case Start_State: //Commands
            parse_debug("Start_State", tok_str);
            
            //Insert Command
            _ptree.insert("command", ltok_str);

            //Go To Command State
            nextState = _ptable[nextState][_keywords[ltok_str]];
			break;

//Insert Into, Drop Table and Create Table Machine
        case Command: //Came from Start_State expecting "table", "into"
            parse_debug("Command", tok_str);
            if (_keywords[ltok_str] == Command) {
                _ptree.insert("command", ltok_str);
                nextState = _ptable[nextState][Table_Name];
            }
            else nextState = Fail_State;
            break;

//Select Star or Fields Machine
        case Select: //Came from Start_State expecting Star or fields or Quote
            parse_debug("Select", tok_str);
            if (_keywords[ltok_str] == Star) {
                _ptree.insert("fields", tok_str);
                nextState = _ptable[nextState][From];
            }
            else if (_keywords[tok_str] == Quote) {
                nextState = _ptable[nextState][Quote];
                field = true;
            }
            else {
                _ptree.insert("fields", tok_str);
                nextState = _ptable[nextState][Select_Comma];
            }
            break;

        case Select_Comma: //Came from Select or Fields expecting Comma or From
            parse_debug("Select_Comma", tok_str);
            if (_keywords[ltok_str] == From)
                nextState = _ptable[nextState][Table_Name];
            else nextState = _ptable[nextState][Fields];
            break;

        case Fields: //Came from Post Table or Select Comma expecting Field
            parse_debug("Fields", tok_str);
            _ptree.insert("fields", tok_str);
            nextState = _ptable[nextState][Select_Comma];
            break;

        case From: //Came from Select_Comma expecting From
            parse_debug("From", tok_str);
            if (_keywords[ltok_str] == From)
                nextState = _ptable[nextState][Table_Name];
            else nextState = Fail_State;
            break;

//Get Table Name
        case Table_Name: //Came from Start_State (Select *, Load),
                            //Select_Comma or Drop expecting table_name
            parse_debug("Table_Name", tok_str);
            _ptree.insert("table_name", tok_str);
            nextState = _ptable[nextState][Post_Table];
            break;

        case Post_Table: //Came From Table expecting 
                        //"where", "fields", "values"
            parse_debug("Post_Table", tok_str);
            nextState = _ptable[nextState][_keywords[ltok_str]];
            break;

//Getting Values
        case Values: //Came from Post Table expecting Quote or value
            parse_debug("Values", tok_str);
            if (_keywords[tok_str] == Quote)
                nextState = _ptable[nextState][Quote];
            else {
                _ptree.insert("values", tok_str);
                nextState = _ptable[nextState][Insert_Comma];
            }
            break;

        case Insert_Comma: //Came from Where, Value, Quote expecting comma
            parse_debug("Insert_Comma", tok_str);
            nextState = _ptable[nextState][Values];
            break;

//Quote Machine
        case Quote: //Push tok_str to current value
            parse_debug("Quote", tok_str);
            //Insert Field or value
            if (_keywords[tok_str] == Quote) {
                if (field) {
                    field = false;
                    _ptree.insert("fields", word);
                    nextState = _ptable[nextState][Select_Comma];
                }
                else {
                    _ptree.insert("values", word);
                    nextState = _ptable[nextState][Insert_Comma];
                }
                word = "";
            }
            //Keep getting quoted word
            else {
                if (word.size() > 0) word += " " + tok_str;
                else word += tok_str;
                nextState = _ptable[nextState][Quote];
            }
            break;

        case Where: //Came from Post Table or Symbol expecting condition 
            parse_debug("Where", tok_str);
            if (_keywords[tok_str] == Quote) {
                condition += "\"";
                nextState = _ptable[nextState][Quote_C];
            }
            else { 
                condition += tok_str;
                nextState = _ptable[nextState][Symbol]; 
                cond_num++; 
            }
            break;

        case Quote_C: //Came From Where Keep Quoting condition
                                    //expects symbol after
            parse_debug("Quote_C", tok_str);
            if (_keywords[tok_str] == Quote) {
                nextState = _ptable[nextState][Symbol];
                condition[condition.size() - 1] = '\"';
                cond_num++;
            }
            else {
                nextState = _ptable[nextState][Quote_C];
                condition += tok_str + " ";
            }
            break;

        case Symbol://Came from Where or Quote_C 
                        //expecting Symbols(<, >, <=, >= and, or, ')' )
            parse_debug("Symbol", tok_str);
            if (_keywords[tok_str] == Symbol) {
                condition += " " + tok_str + " ";
                nextState = _ptable[nextState][Where];
            }
            else nextState = Fail_State;
            break;

//Fail State
        default:
            parse_debug("Fail State", tok_str);
            _valid_ptree = false;
            return _valid_ptree;
            break;
		}
    }

    _valid_ptree = 
        (curState >= 0 && _ptable[curState][0] == Success_State);
 
    //Valid Amount of Conditions and Parenthesis
    if (_valid_ptree && cond_num > 0) {
        if (cond_num % 2 != 0) _valid_ptree = false;
        if (lp != rp) _valid_ptree = false;
        if (_valid_ptree) _ptree.insert("where", condition);
    }

    if(_debug)
        cout << "C: " << condition
        << " |Num: " << cond_num << "|"<< endl;
    
    return _valid_ptree;
}

void Parser::format_tree() {
    if (_ptree.contains("fields")) {
        formatvec(_ptree["fields"]);
    }
    if (_ptree.contains("values")) {
        formatvec(_ptree["values"]);
    }
    if (_ptree.contains("where")) {
        formatvec(_ptree["where"]);
        parse_condition();
    }
    if (_ptree.contains("table_name")) {
        formatvec(_ptree["table_name"]);
    }
}