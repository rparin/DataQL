#ifndef SQL_H
#define SQL_H

#include "Table.h"
#include "Parser.h"
#include "RPN.h"
#include "Shunt_Yard.h"
#include <fstream>
#include <sstream>

class SQL {
public:
    SQL();
    SQL(string filename);
    void run();
    
private:

    void output();

    string _output_file;
    Parser _parser;
    Vector<string> _commands;
};


#endif //SQL_H
