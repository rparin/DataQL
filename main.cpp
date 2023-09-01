/*
 * Author: Ralph Parin
 * Project: Parser Class & Table Class
 */

#include <iostream>
#include <string>
#include "Table.h"
#include "Parser.h"
#include "RPN.h"
#include "SQL.h"
using namespace std;

int main() {

    SQL mysql("sample.txt");
    mysql.run();

    cout << endl << "End of Main" << endl;
    return 0;
}
