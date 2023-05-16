#ifndef DA_TRAINS_HELPY_H
#define DA_TRAINS_HELPY_H

#include "Reader.h"
#include "Utils.hpp"

using std::string;

class Helpy {
/* ATTRIBUTES */
private:
    Reader reader;

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

    void advanced_mode();
    void guided_mode();
    bool process_command(string& s1, string& s2, string& s3);

    // commands

public:
    void terminal();
};

#endif
