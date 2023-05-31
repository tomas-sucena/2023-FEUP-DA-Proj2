#ifndef DA_TRAINS_HELPY_H
#define DA_TRAINS_HELPY_H

#include "../utils/Reader.h"
#include "../utils/Utils.hpp"
#include "../network/DataGraph.h"

using std::string;

class Helpy {
/* ATTRIBUTES */
private:
    DataGraph graph;
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

    void runAlgorithm(int n);

    // commands
    void changeSelectedGraph();
    void runOtherTSP();
    void runBackTrackingTSP();
    void runApproximationTSP();

public:
    void terminal();
};

#endif
