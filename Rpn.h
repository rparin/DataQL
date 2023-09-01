#ifndef RPN_H
#define RPN_H

#include "../!includes/myQueue.h"
#include "../!includes/myStack.h"
#include "../!includes/myVector.h"
#include "../!includes/STokenizer.h"
#include "../!includes/myVector.h"
#include "../!includes/MMap.h"
#include "../!includes/Map.h"
using namespace std;

class RPN {
public:
    RPN();
    RPN(Queue<Token*> postfix);

    //Evalulate postfix condition
    Vector<long> eval(Map<string, int> map, 
        Vector<MMap<string, long>> table);

private:

    enum TokTypes : int {
        Word = 3, Op = 4,

        //Low to High Precedence
        Or = 5, And = 6, Cond = 7,

        //Operators
        LT = 10, LE, GT, GE, EE
    };

    //Create Map for operators
    void create_op_map();

    //Condition Operators
    Vector<long> equals(Vector<string> fields,
        Map<string, int> map, Vector<MMap<string, long>> table);

    Vector<long> lt(Vector<string> fields,
        Map<string, int> map, Vector<MMap<string, long>> table);

    Vector<long> gt(Vector<string> fields,
        Map<string, int> map, Vector<MMap<string, long>> table);

    Vector<long> le(Vector<string> fields,
        Map<string, int> map, Vector<MMap<string, long>> table);

    Vector<long> ge(Vector<string> fields,
        Map<string, int> map, Vector<MMap<string, long>> table);

    Queue<Token*> _postfix;
    Map<string, int> _operator;
};

#endif //RPN_H