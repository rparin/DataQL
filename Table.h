#ifndef TABLE_H
#define TABLE_H
#include <iomanip>
#include "../!includes/Record.h"
#include "../!includes/myBPlusTree.h"
#include "../!includes/Map.h"
#include "../!includes/MMap.h"
#include "Shunt_Yard.h"
#include "Rpn.h"
class Table {
public:

    Table();

    //Name of File w/o extension name
    //Name of File and Vector of filed names
    Table(const string& name, const vectorstr& field_names);

    //Name of File w/o extension name
    //Takes in file
    Table(const string& name);

    //Deletes Table and File
    void drop();

    //Writes Record into Binary File
    void insert_into(vectorstr field_list);

    //Gets all records from file and prints it
    ostream& select_all(ostream& outs = cout) const;
    void select_all(Table& t);

    //Take in vector of records
    Table get_table(Vector<long> indicies);

    //Execute Select Commands
    Table select(vectorstr field_list);
    Table select(vectorstr field_list, vectorstr conditions);

    vectorstr get_fields();
    Map<string, int> get_fmap();

    Vector<MMap<string, long>> get_indices();

    friend ostream& operator <<(ostream& outs, const Table& t);

private:
    //Read and Write to File
    void open_fileRW(fstream& f, string filename) throw(char*);

    //Writes to File 
    void open_fileW(fstream& f, string filename);

    //Checks if File exists
    bool file_exists(string filename);

    //Write Fields to file
    void set_fields(vectorstr field_list);

    //Gets the Records of Rows matching the field_list given
    Vector<vectorstr> get_rows(vectorstr field_list);

    string _filename;
    string _file; //Name of file with extension
    vectorstr _field_list;
    Map<string, int> _field_map;
    Vector<MMap<string, long>> _indices;
    int _file_count;
    int _rec_num;
};
#endif //TABLE_H
