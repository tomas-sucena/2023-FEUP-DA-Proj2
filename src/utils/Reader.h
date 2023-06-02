#ifndef DA_PROJ2_READER_H
#define DA_PROJ2_READER_H

#include "Utils.hpp"
#include "../network/TSPGraph.h"

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
    void readVertices(TSPGraph &g, const string &path, bool hasHeader);
    void readEdges(TSPGraph &g, const string &path, bool hasHeader);

public:
    TSPGraph read(const string &path, bool hasHeader);
};

#endif // DA_PROJ2_READER_H
