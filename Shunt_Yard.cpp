#include "Shunt_Yard.h"

Shunt_Yard::Shunt_Yard():
	_input(),_infix(),_postfix() {
}

Shunt_Yard::Shunt_Yard(Vector<string> infix) :
	_input(infix), _infix(), _postfix() {

	set_keys();
	_infix = tokenize(infix);
	_postfix = shunting_yard(_infix);
}

void Shunt_Yard::set_infix(Vector<string> infix) {
	*this = Shunt_Yard(infix);
}

Vector<string> Shunt_Yard::get_postfix() {
	Vector<string> postfix;

	Queue<Token*>::Iterator it;
	Token* temp;
	for (it = _postfix.begin(); it != _postfix.end(); it++) {
		temp = *it;
		postfix += temp->token_str();
	}

	return postfix;
}

Queue<Token*> Shunt_Yard::get_pf() {
	return _postfix;
}

Queue<Token*> Shunt_Yard::tokenize(const Vector<string>& input) {
	Queue<Token*> infix;

	for (int i = 0; i < input.size(); i++) {

		//Operators/Conditions
		if(_keys.contains(input[i]))
			infix.push(new Token(input[i], _keys[input[i]]));

		//Words
		else infix.push(new Token(input[i], Word));
	}
	return infix;
}

Queue<Token*> Shunt_Yard::shunting_yard(Queue<Token*> infix) {
	//Tokne* lists to create postfix
	Queue<Token*> postfix;
	Stack<Token*> operators;

	//Token to hold current value in link list
	Token* token;

	//Shunting yard algorithm
	while (!infix.is_empty()) {
		
		//Get token
		token = infix.pop();

		//Words
		if (token->type() == Word) {
			postfix.push(token);
		}

		//Operators
		else if (token->type() > Op) {

			// while there is a operator at 
			//the top of the operator stack
			while (!operators.is_empty() 

				//and the operator at the top of 
				//the operator stack has greater precedence
				&& operators[0]->type() > token->type()) {

				//pop operators from the operator 
				//stack onto the postfix queue
				postfix.push(operators.pop());
			}

			//push token onto the operator stack
			operators.push(token);
		}

		//Left Parentheses
		else if (token->type() == LPar) {
			operators.push(token);
		}

		//Right Parentheses
		else if (token->type() == RPar) {

			//While the top of the operator stack is not a LPar
			Token* top = static_cast<Token*>(operators[0]);
			while (top->type() != LPar) {

				//Pop operator stack onto postfix queue
				postfix.push(operators.pop());
				top = static_cast<Token*>(operators[0]);
			}

			//If top of stack is a LPar
			if (top->type() == LPar) {

				//Discard LPar
				operators.pop();
			}
		}
	}

	//Put rest of operators into postfix
	while (!operators.is_empty()) {
		postfix.push(operators.pop());
	}

	return postfix;
}

void Shunt_Yard::set_keys() {
	_keys.insert("<", Cond);
	_keys.insert(">", Cond);
	_keys.insert(">=", Cond);
	_keys.insert("<=", Cond);
	_keys.insert("=", Cond);
	_keys.insert("And", And);
	_keys.insert("Or", Or);
	_keys.insert("(", LPar);
	_keys.insert(")", RPar);
}
