#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <iomanip>
#include "myVector.h"
using namespace std;

typedef Vector<string> vectorstr;

class Record {
public:
    Record() :
        _recno(-1), _num_of_fields(0), record() {
        for (int i = 0; i < MAX_ROWS; i++)
            record[i][0] = NULL;
    }

    Record(const vectorstr& v) :
        _recno(-1), _num_of_fields(v.size()), record() {
        for (int i = 0; i < MAX_ROWS; i++) record[i][0] = NULL;
        for (int i = 0; i < v.size(); i++) strcpy_s(record[i], v[i].c_str());
    }

    vectorstr get_record() {
        vectorstr v;
        //cout << " get_record(): " << _num_of_fields << endl;
        for (int i = 0; i < MAX_ROWS; i++) {

            //Stop Pushing to vector when no more characters to push
            if (record[i][0] == NULL) break;

            //Add strings to vector
            v.push_back(string(record[i]));
        }
        return v;
    }

    vectorstr get_record(Vector<int> indices) {
        vectorstr v = get_record();

        //Return specific items in indices only
        vectorstr vec;
        for (int i = 0; i < indices.size(); i++) {
            vec += v[indices[i]];
        }

        return vec;
    }

    long write(fstream& outs) {
        //write to the end of the file.
        outs.seekg(0, outs.end);
        long pos = outs.tellp();

        //outs.write(&record[0], sizeof(record));
        outs.write(&record[0][0], sizeof(record));

        return pos / sizeof(record);
    }

    long read(fstream& ins, long recno) {
        long pos = recno * sizeof(record);
        ins.seekg(pos, ios_base::beg);

        ins.read(&record[0][0], sizeof(record));
        return ins.gcount();
    }

    friend ostream& operator<<(ostream& outs, const Record& r) {
        for (int i = 0; i < r._num_of_fields; i++) {
            outs << r.record[i] << "|";
        }
        return outs;
    }

private:
    static const int MAX_ROWS = 20;
    static const int MAX_COLS = 50;
    int _recno;
    int _num_of_fields;
    char record[MAX_ROWS][MAX_COLS];
};

#endif //RECORD_H
