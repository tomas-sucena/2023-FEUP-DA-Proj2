#ifndef DA_TRAINS_HELPY_H
#define DA_TRAINS_HELPY_H

#include "Reader.h"
#include "Utils.hpp"

using std::string;

class Helpy {
/* ATTRIBUTES */
private:
    Reader reader;
    uMap<string, int> stationIDs;
    uMap<int, string> stationNames;

    RailGraph graph;
    uMap<string, uSet<string>> districts, municipalities, trainLines;

    // maps used to process commands
    static std::map<string, int> command, target, what;

/* CONSTRUCTOR */
public:
    Helpy();

/* METHODS */
private:
    void fetchData();

    static string readInput(const string& instruction, uSet<string>& options);
    static double readNumber(const string& instruction);
    static void readInputFromTable(std::vector<Edge*> ref, std::list<Edge*>& edges);
    string readStation();
    string readLocation(const string& instruction);
    string readLocation();

    void advanced_mode();
    void guided_mode();
    bool process_command(string& s1, string& s2, string& s3);

    // commands

public:
    void terminal();
};

#endif
