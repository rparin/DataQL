#include "State_Machine.h"

void init_table(int _table[][MAX_COLUMNS])
{
	//Initialize table to -1
	for (int i = 0; i < MAX_ROWS; i++) {
		for (int j = 0; j < MAX_COLUMNS; j++) {
			_table[i][j] = -1;
		}
	}
}

void mark_success(int _table[][MAX_COLUMNS], int state)
{
	_table[state][0] = 1;
}

void mark_fail(int _table[][MAX_COLUMNS], int state)
{
	_table[state][0] = 0;
}

bool is_success(int _table[][MAX_COLUMNS], int state)
{
	if (_table[state][0]) return true;
	else return false;
}

void mark_cells(int row, int _table[][MAX_COLUMNS], 
	char from, char to, int state)
{
	for (int i = static_cast<int>(from);
		i <= static_cast<int>(to); i++) {
		_table[row][i] = state;
	}
}

void mark_cells(int row, int _table[][MAX_COLUMNS], 
	const char columns[], int state)
{
	int len = static_cast<int>(strlen(columns));
	for (int i = 0; i < len; i++) {
		_table[row][columns[i]] = state;
	}
}

void mark_cell(int row, int table[][MAX_COLUMNS], 
	int column, int state)
{
	table[row][column] = state;
}

void print_table(int _table[][MAX_COLUMNS])
{
	for (int i = 0; i < MAX_ROWS; i++) {
		for (int j = 0; j < MAX_COLUMNS; j++) {
			cout << _table[i][j] << " ";
		}
		cout << endl;
	}
}

void show_string(char s[], int _pos)
{
	int len = static_cast<int>(strlen(s));

	//Print String
	for (int i = 0; i < len; i++) {
		cout << s[i];
	}
	//Print position number
	cout << "\tpos: " << _pos << endl;

	//Print arrow under string to indicate position
	for (int i = 0; i <= _pos; i++) {
		if (i == _pos) cout << "^";
		else cout << " ";
	}
	cout << endl;
}
