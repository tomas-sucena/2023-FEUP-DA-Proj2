#ifndef DA_TRAINS_HELPY_H
#define DA_TRAINS_HELPY_H

#include "../utils/Reader.h"
#include "../utils/Utils.hpp"
#include "../network/TSPGraph.h"

using std::string;

class Helpy {
/* ATTRIBUTES */
private:
    TSPGraph graph;
    Reader reader;
    string pathToRoot;

    // maps used to process commands
    static std::map<string, int> command, target, what;

/* CONSTRUCTOR */
public:
    Helpy();

/* METHODS */
private:
    void fetchData(const string &path, bool hasHeader);

    static string readInput(const string& instruction, uSet<string>& options);
    static double readNumber(const string& instruction);

    void advancedMode();
    void guidedMode();
    bool processCommand(string& s1, string& s2, string& s3);

    static void printPath(std::list<std::pair<int, double>> &path);
    void runAlgorithm(int n);

    // commands
    void changeSelectedGraph();

public:
    void terminal();
};

#endif
