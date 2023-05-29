#include "Reader.h"

#include "../network/Place.hpp"

/**
 * @brief creates a Reader object
 * @param path path to the directory where the files to be read are
 */
Reader::Reader(string path, char valueDelim, char lineDelim) : valueDelim(valueDelim), lineDelim(lineDelim) {
    setPath(path);
}

/**
 * @brief extracts the next value of a CSV line
 * @complexity 0(n)
 * @param lineIt iterator of the CSV line
 * @param value string where the extracted value will be placed
 * @param delim character that delimits the value to be read
 * @return value
 */
void Reader::extractValue(std::string::iterator& lineIt, std::string& value, char delim){
    // check if the value is delimited by quotation marks
    if (*lineIt == '"'){
        delim = '"';
        ++lineIt;
    }

    value.clear();
    for (; *lineIt != '\000' && *lineIt != delim; ++lineIt)
        value += *lineIt;

    if (delim == '"') ++lineIt; // skip the quotation mark
    ++lineIt; // skip the delimiter
}

/**
 * @brief reads the file which contains information about the Edges
 * @param graph Undirected graph that will be modelled based on the read information
 */
void Reader::readEdges(DataGraph& graph){
    reader.open(path + "stations.csv");
    int n = 0;

    string line;
    getline(reader, line); // header

    for (int i = 1; getline(reader, line); ++i){
        auto it = line.begin();

        // read the origin
        string origin;
        extractValue(it, origin, valueDelim);
        int ori = stoi(origin);

        // read the destination
        string destination;
        extractValue(it, destination, valueDelim);
        int dest = stoi(destination);

        // read the distance
        string distance;
        extractValue(it, distance, valueDelim);
        double dist = stof(distance);


        // check if the station is repeated
        if(n != ori){
            n = ori;
            // add the station to the graph
            graph.addVertex(new Place(0,0));
        }
        graph.addEdge(ori, dest, dist);

    }

    reader.close();
    reader.clear();
}


