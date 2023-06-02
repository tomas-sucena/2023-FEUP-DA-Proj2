#include "Reader.h"

/**
 * @brief creates a Reader object
 * @param valueDelim character that delimits each value in a line
 * @param lineDelim character that delimits each line in the file
 */
Reader::Reader(char valueDelim, char lineDelim) : valueDelim(valueDelim), lineDelim(lineDelim) {}

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
 * @brief reads the file which contains information about the vertices of the graph
 * @param graph undirected graph that will be modelled based on the read information
 */
void Reader::readVertices(TSPGraph &graph, const string &path, bool hasHeader) {
    reader.open(path);

    string line;
    if (hasHeader) getline(reader, line); // header

    while (getline(reader, line)){
        auto it = line.begin();

        // read the id (will be ignored)
        string _;
        extractValue(it, _, valueDelim);

        // read the destination
        string longitude;
        extractValue(it, longitude, valueDelim);

        // read the distance
        string latitude;
        extractValue(it, latitude, lineDelim);

        graph.addVertex(new Place(stod(latitude), stod(longitude)));
    }

    reader.close();
    reader.clear();
}

/**
 * @brief reads the file which contains information about the edges of the graph
 * @param graph undirected graph that will be modelled based on the read information
 */
void Reader::readEdges(TSPGraph &graph, const string &path, bool hasHeader) {
    reader.open(path);

    string line;
    if (hasHeader) getline(reader, line); // header

    while (getline(reader, line)){
        auto it = line.begin();

        // read the origin
        string source;
        extractValue(it, source, valueDelim);

        // read the destination
        string destination;
        extractValue(it, destination, valueDelim);

        // read the distance
        string distance;
        extractValue(it, distance, lineDelim);

        int src = stoi(source) + 1;
        int dest = stoi(destination) + 1;

        graph.resize(std::max(src, dest));
        graph.addEdge(src, dest, stod(distance));
    }

    reader.close();
    reader.clear();
}

/**
 * @brief reads a file which represents a graph
 * @param path path to the file/directory where the data files are
 * @param twoFiles
 * @param hasHeader
 * @return undirected graph modelled after the file
 */
TSPGraph Reader::read(const string &path, bool hasHeader) {
    TSPGraph graph;
    bool oneFile = (path.substr(path.size() - 4, 3) == ".csv");

    if (oneFile) {
        readEdges(graph, path, hasHeader);
        return graph;
    }

    std::string path_ = (path.back() == '/') ? path : path + '/';

    readVertices(graph, path_ + "nodes.csv", hasHeader);
    readEdges(graph, path_ + "edges.csv", hasHeader);

    return graph;
}


