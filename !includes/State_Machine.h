#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <iostream>
#include <string.h> // use for strlen
using namespace std;

//Table Value
static const int MAX_ROWS = 50;
static const int MAX_COLUMNS = 130;

//Key Origin
enum keyTypes : int {
    Unknown = 0, Alpha, Num, Punct, Space, Dec, Sym, Op, Period = 8
};

//Key Types
static const char ALPHA[] =
"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char DIGITS[] = "0123456789";
static const char PUNCT[] = "\'\",:;?";
static const char SYMB[] = "!#$%&()\\@[]^_`{}|~";
static const char OP[] = "*+-/<>=";
static const char SPACE = ' ';
static const char PERIOD = '.';

//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS]);

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state);

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state);

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state);

//Mark a range of cells in the array. 
void mark_cells(int row, int _table[][MAX_COLUMNS], 
	char from, char to, int state);

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], 
	const char columns[], int state);

//Mark this row and column
void mark_cell(int row, int table[][MAX_COLUMNS], 
	int column, int state);

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS]);

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos);

#endif //STATE_MACHINE_H
