#include "STokenizer.h"
int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

STokenizer::STokenizer()
{
	make_table();
	_pos = 0;
}

STokenizer::STokenizer(char str[])
{
	make_table();
	set_string(str);
}

STokenizer::STokenizer(string str)
{
	make_table();
	set_string(str.c_str());
}

bool STokenizer::done()
{
	//True if the position is at the end of the cstring
	return !more();
}

bool STokenizer::more()
{
	//return !done();
	return _pos <= _len;
}

void STokenizer::set_string(const char* str)
{
	_pos = 0;
	_len = static_cast<int>(strlen(str));
	assert(MAX_BUFFER >= _len);

	//Length of new string
	int temp = 0;
	for (int i = 0; i < _len; i++) {
		
		_buffer[temp] = str[i];

		//Increase length of string
		temp++;
	}
	_len = temp;

	//Set New End Point
	_buffer[_len] = NULL;
}

void STokenizer::make_table()
{
	//Initialize table to -1
	init_table(_table);

	//Setting which states are valid
	mark_success(_table, Alpha);
	mark_success(_table, Num);
	mark_success(_table, Punct);
	mark_success(_table, Space);
	mark_success(_table, Dec);
	mark_success(_table, Sym);
	mark_success(_table, Op);

	//Alphabet
	mark_cells(0,_table, ALPHA, Alpha);
	mark_cells(Alpha, _table, ALPHA, Alpha);

	//Punctuation
	mark_cells(0,_table,PUNCT, Punct);

	//Symbol
	mark_cells(0, _table,SYMB, Sym);

	//Operator
	mark_cells(0, _table,OP, Op);

	//Space
	mark_cell(0,_table,SPACE, Space);
	mark_cell(Space,_table,SPACE, Space);

	//Numbers
	mark_cells(0,_table,DIGITS, Num);
	mark_cells(Num,_table,DIGITS, Num);
	mark_cell(Num,_table,PERIOD, Period);
	mark_cells(Period,_table,DIGITS, Dec);
	mark_cells(Dec,_table,DIGITS, Dec);
}

bool STokenizer::get_token(int& end_state, string& token)
{
	int asciVal;
	string tempToken = "";
	int startState = 0;
	int curState = 0;

	//Increase position if position is at the end
	//Indicate no more tokens to be recieved
	for (int i = _pos; i <= _len; i++) {
		//Convert char to ascii value
		asciVal = static_cast<int>(_buffer[i]);

		//Spanish Characters or values not in ASCII Table
		if (static_cast<int>(asciVal < 0)) {
			end_state = startState;
			token = tempToken;

			if (tempToken.length() == 0) {
				token = _buffer[i];
				_pos++;
			}

			return true;
		}

		//Get current State and pos
		curState = _table[startState][asciVal];
		_pos = i;

		//Invalid Character
		if (startState == 0 && curState == -1) {
			token = _buffer[i];
			_pos++;
			return true;
		}

		//Return Token
        if (curState == -1 || _buffer[i] == NULL) {
			end_state = startState;
			token = tempToken;
			if (_buffer[i] == NULL) return false;
			else return true;
		}

		//Store valid char and update state
        if (curState > 0) {
			tempToken += _buffer[i];
			startState = _table[startState][asciVal];
			_pos++;
		}
	}

	return false;
}

STokenizer& operator>>(STokenizer& s, Token& t)
{
	//Extract token
	string new_str = "";
	int new_type = 0;
	s.get_token(new_type, new_str);

	//Set Token t to new Token
	Token new_tok(new_str, new_type);
	t = new_tok;

	//Return Refrence to s
	return s;
}
