#ifndef DA_PROJ2_READER_H
#define DA_PROJ2_READER_H

#include "Utils.hpp"
#include "../network/DataGraph.h"

using std::string;

class Reader {
/* ATTRIBUTES */
private:
    // for reading
    std::ifstream reader;
    char valueDelim, lineDelim; // delimiters

/* CONSTRUCTOR */
public:
    explicit Reader(char valueDelim = ',', char lineDelim = '\n');

/* METHODS */
private:
    static void extractValue(std::string::iterator& lineIt, std::string& value, char delim);
    void readEdges(DataGraph& g, const string &path, bool hasHeader);

public:
    DataGraph read(const string &path, bool hasHeader);
};

#endif // DA_PROJ2_READER_H
