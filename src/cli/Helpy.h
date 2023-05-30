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

    // maps used to process commands
    static std::map<string, int> command, target, what;

/* CONSTRUCTOR */
public:
    Helpy();

/* METHODS */
private:
    void fetchData(string type, string what);

    static string readInput(const string& instruction, uSet<string>& options);
    static double readNumber(const string& instruction);

    void advanced_mode();
    void guided_mode();
    bool process_command(string& s1, string& s2, string& s3);

    // commands
    void changeSelectedGraph();
    void runOtherTSP();
    void runBackTrackingTSP();
    void runApproximationTSP();

public:
    void terminal();
};

#endif
