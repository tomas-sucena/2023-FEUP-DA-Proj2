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
    string path;
    char valueDelim, lineDelim; // delimiters

/* CONSTRUCTOR */
public:
    explicit Reader(std::string path, char valueDelim = ',', char lineDelim = '\n');

/* METHODS */
private:
    static void extractValue(std::string::iterator& lineIt, std::string& value, char delim);
    void readEdges(DataGraph& g, string type, string what);

public:
    DataGraph read(string type, string what);
    std::string getPath() const;
    void setPath(std::string& path);
};

#endif // DA_PROJ2_READER_H
