#include "Rpn.h"

RPN::RPN() :
	_postfix(), _operator() {
}

RPN::RPN(Queue<Token*> postfix):
	_postfix(postfix), _operator(){
	create_op_map();
}

Vector<long> RPN::eval(Map<string, int> map, 
	Vector<MMap<string, long>> table) {

	//Create a copy so push and pop don't affect postfix
	Queue<Token*> postfix = _postfix;

	Stack<Vector<long>> record_num;

	//Get Fields
	Vector<string> fields;

	//Hold Current value
	Token* tok;
	Queue<Token*>::Iterator it;

	//RPN Algorithm
	while (!postfix.is_empty()) {
		//Get Current Token from postfix
		tok = postfix.pop();
		//Values
		if (tok->type() == Word) {
			fields += tok->token_str();
		}

		//Logical Conditions
		else if (tok->type() == Cond) {

			//Equals
			if (_operator[tok->token_str()] == EE) {
				record_num.push(equals(fields, map,table));
			}

			//>=
			else if (_operator[tok->token_str()] == GE) {
					record_num.push(ge(fields, map, table));
			}
			//>
			else if (_operator[tok->token_str()] == GT) {
				record_num.push(gt(fields, map, table));
			}
			//<=
			else if (_operator[tok->token_str()] == LE) {
				record_num.push(le(fields, map, table));
			}
			//<
			else if (_operator[tok->token_str()] == LT) {
				record_num.push(lt(fields, map, table));
			}
			fields.clear();
		}

		//Intersection of record numbers
		else if (tok->type() == And) {
			record_num.push(
				intrsectVect(record_num.pop(), record_num.pop()));
		}

		//Union of record numbers
		else if (tok->type() == Or) {
			record_num.push(
				unionVec(record_num.pop(), record_num.pop()));
		}
	}

	//return answer.pop();
	return record_num.pop();
}

void RPN::create_op_map() {
	_operator.insert("<", LT);
	_operator.insert("<=", LE);
	_operator.insert("=", EE);
	_operator.insert(">", GT);
	_operator.insert(">=", GE);
}

Vector<long> RPN::equals(Vector<string> fields, 
	Map<string, int> map, Vector<MMap<string, long>> table){
	bool debug = false;
	if(debug) cout << "||" << fields[1] << "||" << endl;

	return table[map[fields[0]]][fields[1]];
}

Vector<long> RPN::lt(Vector<string> fields, Map<string, int> map,
	Vector<MMap<string, long>> table) {
	//Store Records
	Vector<long> rec;

	//Iterator to get items
	MMap<string, long>::Iterator it =
		table[map[fields[0]]].begin();
	MMap<string, long>::Iterator temp = it;
	//Pair used to get records
	MPair<string, long> pair;

	//Go Through Records
	for (; it != table[map[fields[0]]].lower_bound(fields[1]); it++) {
		pair = *it;
		rec += table[map[fields[0]]][pair.key];
	}

	return rec;
}

Vector<long> RPN::gt(Vector<string> fields, Map<string, int> map, 
	Vector<MMap<string, long>> table){
	
	//Store Records
	Vector<long> rec;

	//Iterator to get items
	MMap<string, long>::Iterator it = 
		table[map[fields[0]]].lower_bound(fields[1]);

	//Pair used to get records
	MPair<string, long> pair;

	//Go Through Records
	for (; it != table[map[fields[0]]].end(); it++) {
		pair = *it;
		rec += table[map[fields[0]]][pair.key];
	}

	return rec;
}

Vector<long> RPN::le(Vector<string> fields, Map<string, int> map, 
	Vector<MMap<string, long>> table) {
	//Store Records
	Vector<long> rec;

	//Iterator to get items
	MMap<string, long>::Iterator it =
		table[map[fields[0]]].begin();

	//Pair used to get records
	MPair<string, long> pair;

	//Go Through Records
	for (; it != table[map[fields[0]]].end(); it++) {
		pair = *it;
		rec += table[map[fields[0]]][pair.key];

		if (it == table[map[fields[0]]].lower_bound(fields[1])) break;
	}

	return rec;
}

Vector<long> RPN::ge(Vector<string> fields, Map<string, int> map, 
	Vector<MMap<string, long>> table) {
	//Store Records
	Vector<long> rec;
	MMap<string, long>::Iterator it;

	//Pair to get records
	MPair<string, long> pair;
	for (it = table[map[fields[0]]].lower_bound(fields[1]);
		it != table[map[fields[0]]].end(); it++) {
		pair = *it;

		//Push Record to Records
		rec += table[map[fields[0]]][pair.key];
	}

	return rec;
}

