#include "SQL.h"

SQL::SQL():
    _parser(), _commands(){
    
}

SQL::SQL(string filename): 
    _output_file(filename + "_Output.txt"),
    _parser(), _commands() {

    //Open File 
    ifstream inFile;
    inFile.open(filename);

    //File Not Found
    assert(inFile.fail() == false);

    stringstream ss;
    string line;
    getline(inFile, line);

    while (getline(inFile, line)) {
        _commands += line;
    }

    //Create Output File
    output();
}

void SQL::run() {
    Parser p;
    string input;
    ofstream outFile;
    bool debug = false;
    bool file = (_commands.size() != 0);
    bool valid = false;

    int c_index = 0;
    while (input != "exit") {
        c_index++; input = "";
        if (c_index > _commands.size()) break;

        //Get Command from user or file
        if (file) input = _commands[c_index];
        else { cout << endl << " >> "; getline(cin, input); }

        if (file) {
            outFile << input << endl;
        }

        //Comments
        if (input.size() <= 3 || (input.size() > 3 &&
            input[0] == '/' && input[1] == '/')) {
            cout << input << endl;
            
        }

        //Parse Input
        else if (input.size() != 0) {

            p.set_string(input);
            MMap<string, string> ptree = p.get_ptree();

            if (debug) cout << ptree
                << "Tree is valid: " << boolalpha
                << p.is_valid() << endl << endl;

            if (p.is_valid()) {
                cout << input << endl;

                //Select Where
                if (ptree.contains("where")) {
                    Table t(ptree["table_name"][0]);
                    cout << t.select(ptree["fields"], ptree["where"]) << endl;
                    outFile << t.select(ptree["fields"], ptree["where"]) << endl;
                }

                //Select
                else if (ptree["command"].contains("select")) {
                    Table t(ptree["table_name"][0]);
                    cout << t.select(ptree["fields"]) << endl;
                    outFile << t.select(ptree["fields"]) << endl;
                }

                //Insert Into
                else if (ptree["command"] == vectorstr({ "insert", "into" })) {
                    Table t(ptree["table_name"][0]);
                    t.insert_into(ptree["values"]);
                }

                //Create Table
                else if (ptree["command"] == vectorstr({ "create", "table" }) ||
                    ptree["command"] == vectorstr({ "make", "table" })) {
                    Table t(ptree["table_name"][0], ptree["fields"]);
                }

                //Drop Table
                else if (ptree["command"] == vectorstr({ "drop", "table" })) {
                    Table t(ptree["table_name"][0]);
                    t.drop();
                }
            }
        }
    }
}

void SQL::output() {
    ofstream _outFile;

    //create file to write to
    _outFile.open(_output_file);
}
