#include "Table.h"

Table::Table() :
    _filename(""),_file(""),
    _file_count(0), _rec_num(0),
    _indices(), _field_map(), _field_list() {
}

Table::Table(const string& name, 
    const vectorstr& field_names):
    _filename(name), _file(name + ".bin"),
    _file_count(0), _rec_num(0) {
    set_fields(field_names);

    fstream f;

    //Creates file, overwrites if file already exists
    open_fileW(f, _file);

    //Writes Fields into file
    Record rec(_field_list);
    rec.write(f);

    //close file
    f.close();
}

Table::Table(const string& name):
    _filename(name),
    _file(name + ".bin"),
    _file_count(0), _rec_num(0) {
    fstream f;

    Record r; int recno = 0;
    open_fileRW(f, _file); 

    //read and set fields
    r.read(f, recno);
    set_fields(r.get_record());

    //Read First Record
    recno++; r.read(f, recno);

    //Store Records
    vectorstr records;

    //Go through file
    while (!f.eof()) {

        //Save records to Indices
        records = r.get_record();
        for (int i = 0; i < records.size(); i++) {
            _indices[i].insert(records[i], recno);
        }

        recno++;

        //Read Next Record
        r.read(f, recno);
    }
    _rec_num += recno;
    _rec_num--;

    //Closes file
    f.close();
}

void Table::drop() {

    //Clear Variables
    _field_list.clear();
    _field_map.clear();
    _indices.clear();
    _file_count = 0;
    _rec_num = 0;
    remove(_file.c_str()); //Delete File
}

void Table::insert_into(vectorstr field_list) {

    fstream f;

    if (field_list.size() == _field_list.size()) {
        //Opens file to append to
        open_fileRW(f, _file);

        //Writes entry list to file
        Record rec(field_list);
        long recno = rec.write(f);

        //Save to Indices
        for (int i = 0; i < field_list.size(); i++) {
            _indices[i].insert(field_list[i], recno);
        }

        //Closes file
        f.close();
        _rec_num++;
    }
}

ostream& Table::select_all(ostream& outs) const{
    Record r;
    fstream f;
     
    int recno = 0;

    f.open(_file, std::fstream::in | std::fstream::binary);

    outs << "Table Name: " << _filename << ", Records: " << _rec_num;

    //Read First Record
    r.read(f, recno);

    outs << endl;
    vectorstr records;
    while (!f.eof()) {
        outs << endl;

        //Store Record
        records = r.get_record();

        //Print Record
        outs << right << setw(10);
        if (recno == 0) outs << "Records";
        else outs << recno;

        for (int i = 0; i < records.size(); i++) {
            outs << right << setw(20) << records[i];
        }

        recno++;

        //Read Next Record
        r.read(f, recno);
    }

    //Closes file
    f.close();

    return outs;
}

void Table::select_all(Table& t){
    Record r; fstream f;
    int recno = 0;
    open_fileRW(f, _file);

    //Skip the First Record
    r.read(f, recno); recno++; r.read(f, recno);

    while (!f.eof()) {

        //Insert Record Record
        t.insert_into(r.get_record());
        recno++;

        //Read Next Record
        r.read(f, recno);
    }

    //Closes file
    f.close();
}

Table Table::get_table(Vector<long> indicies) {


    fstream f;
    //Create Table
    _file_count++;
    Table t("Z_" + _filename + to_string(_file_count), _field_list);

    //Get Records From File
    Record r;
    open_fileRW(f, _file);
    for (int i = 0; i < indicies.size(); i++) {

        //Go to the Record
        r.read(f, indicies[i]);

        //Insert Record Into Table
        t.insert_into(r.get_record());
    }
    f.close();

    return t;
}

Table Table::select(vectorstr field_list) {
    _file_count++;

    //Store Row of Records
    Vector<vectorstr> records;

    //All
    if (field_list.size() >= 0 && field_list[0] == "*") {
        records = get_rows(_field_list);
    }

    //Certain Fields
    else {
        records = get_rows(_field_list);
    }

    //Create Table
    Table t(_filename + to_string(_file_count), _field_list);

    //Insert Fields to table
    for (int i = 0; i < records.size(); i++) {
        t.insert_into(records[i]);
    }

    return t;
}

Table Table::select(vectorstr field_list, vectorstr conditions) {

    vectorstr fields;

    //All
    if (field_list.size() >= 0 && field_list[0] == "*") {
        fields = _field_list;
    }

    //Get Post fix of conditions
    Shunt_Yard yard(conditions);

    //Set postfix
    RPN rpn(yard.get_pf());
    return get_table(rpn.eval(_field_map,_indices));
}

void Table::set_fields(vectorstr list) {

    //Clear current fields
    _field_list.clear();
    _field_map.clear();
    _indices.clear();

    //Set new fields
    for (int i = 0; i < list.size(); i++) {
        _field_list += list[i];
        _field_map.insert(list[i], i);

        //Create indices
        _indices += MMap<string, long>();
    }



}

Vector<vectorstr> Table::get_rows(vectorstr field_list) {

    Vector<vectorstr> rows;
    bool all = (field_list[0] == "*");

    //Hold Which Indices selected
    Vector<int> field_numbers;
    if (!all) {
        for (int i = 0; i < field_list.size(); i++) {
            field_numbers += _field_map[field_list[i]];
        }
    }

    //Open File
    Record r; int recno = 0; fstream f;
    open_fileRW(f, _file);

    //Skip the First Record
    r.read(f, recno); recno++; r.read(f, recno);

    //Go Through records
    while (!f.eof()) {

        //Get All Fields
        if(all) rows += r.get_record();

        //Get Records of specific fields
        else rows += r.get_record(field_numbers);

        recno++; //Increase record number

        //Read Next Record
        r.read(f, recno);
    }

    //Close file
    f.close();

    return rows;
}

vectorstr Table::get_fields() {
    return _field_list;
}

Map<string, int> Table::get_fmap() {
    return _field_map;
}

Vector<MMap<string, long>> Table::get_indices() {
    return _indices;
}

void Table::open_fileRW(fstream& f, string filename) throw(char*) {
    const bool debug = false;

    //File doesn't exist
    //Create a file
    if (!file_exists(filename)) {
        f.open(filename, std::fstream::out | std::fstream::binary);

        cout << "file open (RW) failed: with out|" 
             << filename << "]" << endl;
        throw("file RW failed  ");
    }

    //File exists and opens it
    else {
        f.open(filename, std::fstream::in 
                        | std::fstream::out 
                        | std::fstream::binary);
    }
}

void Table::open_fileW(fstream& f, string filename) {
    f.open(filename,std::fstream::out | std::fstream::binary);

    if (f.fail()) {
        cout << "file open failed: " << filename << endl;
        throw("file failed to open.");
    }

}

bool Table::file_exists(string filename) {

    const bool debug = false;
    fstream ff;
    ff.open(filename, std::fstream::in | std::fstream::binary);
    if (ff.fail()) {
        if (debug) cout 
            << "file_exists(): File does NOT exist: " 
            << filename << endl;
        return false;
    }
    if (debug) cout 
        << "file_exists(): File DOES exist: " 
        << filename << endl;
    ff.close();

    return true;
}

ostream& operator<<(ostream& outs, const Table& t) {
    bool debug = false;
    //Print file contents if file exists
    fstream ff;
    ff.open(t._file, std::fstream::in | std::fstream::binary);
    if (!ff.fail()) t.select_all(outs);
    ff.close();

    if (debug) {
        for (int i = 0; i < t._indices.size(); i++) {
            cout << t._field_list[i] << endl;
            cout << t._indices[i];
        }
        cout << endl;
    }


    return outs;
}
