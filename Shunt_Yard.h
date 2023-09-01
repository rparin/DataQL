#ifndef SHUNT_YARD
#define SHUNT_YARD
#include "../!includes/STokenizer.h"
#include "../!includes/myQueue.h"
#include "../!includes/myStack.h"
#include "../!includes/myVector.h"
#include "../!includes/Map.h"
using namespace std;

class Shunt_Yard
{
public:

	//Constructors
	Shunt_Yard();
	Shunt_Yard(Vector<string> infix);

	//Setters
	void set_infix(Vector<string> infix);

	//Getters
	Vector<string> get_postfix();

	Queue<Token*> get_pf();
	Queue<Token*> _infix;
	Queue<Token*> _postfix;
private:

	//Takes in Equation String, Return Infix Queue
	Queue<Token*> tokenize(const Vector<string>& infix);

	//Takes in infix queue, Returns postfix Queue
	Queue<Token*> shunting_yard(Queue<Token*> infix);

	void set_keys();

	enum TokTypes : int {
		LPar = 1, RPar, Word, Op = 4,

		//Low to High Precedence
		Or = 5, And = 6, Cond = 7
	};

	Vector<string> _input;

	Map<string, int> _keys;
};


#endif //SHUNT_YARD
