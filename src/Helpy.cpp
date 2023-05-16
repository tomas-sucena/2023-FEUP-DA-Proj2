#include "Helpy.h"

using std::cout;
using std::endl;

// output colors
#define RESET   "\033[0;m"
#define RED     "\033[1;31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define YELLOW  "\033[33m"
#define BOLD    "\033[1m"

// text
#define DASHED_LINE "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
#define BREAK       endl << YELLOW << DASHED_LINE << RESET << endl << endl
#define YES_NO      " (" << GREEN << "Yes" << RESET << '/' << RED << "No" << RESET << ')'

std::map<string, int> Helpy::command = {{"display", 1}, {"print", 1}, {"show", 1}, {"calculate", 2},
                                        {"calc", 2}, {"determine", 2}, {"change", 3}, {"switch", 3}};

std::map<string, int> Helpy::target = {{"all", 4}, {"data", 6}, {"maximum", 8},{"max", 8}, {"affected", 10},
                                       {"operating", 12}, {"busiest", 14}, {"railway",16}};

std::map<string, int> Helpy::what = {{"directory", 17}, {"dir", 17}, {"train", 20}, {"trains", 20},
                                     {"station", 23},{"stations", 23},{"district", 23},{"districts", 23},
                                     {"municipality", 23},{"municipalities", 23}, {"pairs", 26}, {"pair", 26},
                                     {"sources", 30}, {"source", 30},{"sinks", 33},{"sink", 33},
                                     {"network", 36}};

/**
 * @brief creates a new Helpy object
 */
Helpy::Helpy() : reader("../data", ',') {
    fetchData();
}

/**
 * @brief reads and parses the data files
 */
void Helpy::fetchData() {
    // create the graph
    graph = reader.read();
    graph.railwaySources = reader.getRailwaySources();
    graph.railwaySinks = reader.getRailwaySinks();

    // fill the data structures
    stationIDs = reader.getStationIDs();
    stationNames = reader.getStationNames();
    districts = reader.getDistricts();
    municipalities = reader.getMunicipalities();
    trainLines = reader.getTrainLines();
}

/**
 * @brief reads a line of user input
 * @param instruction the instruction that will be displayed before prompting the user to type
 * @param options the options that will be displayed to the user
 * @return read input
 */
string Helpy::readInput(const string& instruction, uSet<string>& options){
    string res;
    bool valid = false;

    while (true){
        cout << BREAK;
        cout << instruction << endl << endl;

        string line; getline(std::cin >> std::ws, line);
        Utils::lowercase(line);

        std::istringstream line_(line);

        while (line_ >> res){
            if (options.find(res) == options.end())
                continue;

            valid = true;
            break;
        }

        if (valid) break;

        cout << BREAK;
        cout << RED << "Invalid command! Please, try again." << RESET << endl;
    }

    return res;
}

/**
 * @brief reads a number from the console
 * @param instruction the instruction that will be displayed before prompting the user to input the number
 * @return the number inputted by the user
 */
double Helpy::readNumber(const string &instruction){
    double res;
    bool valid = false;

    while (true){
        cout << BREAK;
        cout << instruction << endl << endl;

        string line; getline(std::cin >> std::ws, line);
        Utils::lowercase(line);

        std::istringstream line_(line);

        string temp;
        while (line_ >> temp){
            try {
                res = stod(temp);
            }
            catch(...){
                continue;
            }

            valid = true;
            break;
        }

        if (valid) break;

        cout << BREAK;
        cout << RED << "Invalid input! Please, try again." << RESET << endl;
    }

    return res;
}

/**
 * @brief reads input related to a previously printed table
 * @param ref vector containing the edges that were displayed in the table
 * @param edges list which will contain the edges to be invalidated
 */
void Helpy::readInputFromTable(std::vector<Edge*> ref, std::list<Edge*>& edges){
    cout << BREAK;
    cout << "Please type the " << BOLD << "indices" << RESET << " of the " << YELLOW << "railways" << RESET
         << " you would like to " << RED << "remove" << RESET << ", separated by a comma (ex: 1,2,7,...)."
         << endl << endl;

    string line; getline(std::cin, line);
    line += ",";

    std::istringstream line_(line);

    for (string temp; getline(line_, temp, ',');){
        int k = std::stoi(temp);
        if (k <= 0 || k > (int) ref.size()) continue;

        ref[k - 1]->valid = false;
        edges.emplace_back(ref[k - 1]);
    }
}

/**
 * @brief reads the name of a Station from the console
 * @complexity O(n)
 * @return the name of the Station
 */
string Helpy::readStation(){
    string station;

    while (true){
        cout << BREAK;
        cout << "Please type the " << BOLD << "name" << RESET << " of the " << YELLOW << "station" << RESET << ":"
             << endl << endl;

        getline(std::cin >> std::ws, station);
        Utils::lowercase(station);
        
        if (stationIDs.find(station) != stationIDs.end()){
            break;
        }

        cout << BREAK;
        cout << RED << "Invalid input! The station you entered does not exist. Please, try again." << RESET << endl;
    }

    return station;
}

/**
 * @brief reads a location, prints the stations that are situated there and prompts the user to choose one of them
 * @param instruction instruction that will be displayed in the console
 * @return name of the station chosen by the user
 */
string Helpy::readLocation(const string& instruction){
    uSet<string> options = {"name", "district", "municipality", "line"};
    string choice = readInput(instruction, options);

    if (choice == "name")
        return readStation();

    uMap<string, uSet<string>> locations;
    if (choice == "district"){
        locations = districts;
    }
    else if (choice == "municipality"){
        locations = municipalities;
    }
    else{
        choice = "train line";
        locations = trainLines;
    }

    // read the station
    string station;

    while (true){
        cout << BREAK;
        cout << "Please type the " << BOLD << "name" << RESET << " of the " << YELLOW << choice << RESET << ':'
             << endl << endl;

        string line; getline(std::cin >> std::ws, line);
        Utils::lowercase(line);

        if (locations.find(line) == locations.end()){
            cout << BREAK;
            cout << RED << "Invalid input! The " << choice << " you entered does not exist. "
                 << "Please, try again." << RESET << endl;

            continue;
        }

        uSet<string> stations = locations[line];

        if (stations.size() == 1){
            station = *stations.begin();
            break;
        }

        Utils::properName(line);

        cout << BREAK;
        cout << BOLD << line << RESET << " has the following " << YELLOW << "stations" << RESET << ':' << endl << endl;

        for (const string& s : stations)
            cout << "* " << s << endl;

        station = readStation();
        break;
    }

    return station;
}

/**
 * @brief reads a location, prints the stations that are situated there and prompts the user to choose one of them
 * @return name of the station chosen by the user
 */
string Helpy::readLocation(){
    std::ostringstream instr;
    instr << "With which of the following would you like to define the desired " << YELLOW << "station"
          << RESET << "?" << endl << endl
          << "* Name" << endl
          << "* District" << endl
          << "* Municipality" << endl
          << "* Line";

    return readLocation(instr.str());
}

/**
 * @brief executes the advanced mode of the UI
 */
void Helpy::advanced_mode(){
b1: cout << BREAK;
    cout << "How can I be of assistance?" << endl << endl;

    string s1, s2, s3;
    std::istringstream s_;

    std::cin >> s1; Utils::lowercase(s1);

    if (s1 == "quit" || s1 == "no" || s1 == "die"){
        goto e1;
    }

    std::cin >> s2;
    Utils::lowercase(s2);

    std::cin >> s3;
    Utils::lowercase(s3);

    if (!process_command(s1, s2, s3)){
        goto b1;
    }

    cout << BREAK;
    cout << "Anything else?" << YES_NO << endl << endl;

    s1.clear(); getline(std::cin >> std::ws, s1);
    Utils::lowercase(s1);

    s_.clear(); s_.str(s1);

    while (s_ >> s1){
        if (s1 == "yes" || s1 == "y"){
            goto b1;
        }
    }

e1: cout << BREAK;
    cout << "See you next time!" << endl << endl;
}

/**
 * @brief executes the guided mode of the UI
 */
void Helpy::guided_mode(){
b2: cout << BREAK;
    cout << "Hello! How can I be of assistance?" << endl;
    cout << endl;
    cout << "* Display" << endl;
    cout << "* Calculate" << endl;
    cout << "* Determine" << endl;
    cout << "* Change" << endl;
    cout << endl;

    string s1, s2, s3;
    std::istringstream s_;

    std::cin >> s1; Utils::lowercase(s1);

    if (s1 == "display"){
        cout << BREAK;
        cout << "* All" << endl;
        cout << "* Busiest" << endl;
        cout << "* Data" << endl;
        cout << "* Railway" << endl;
    }
    else if (s1 == "calculate"){
        cout << BREAK;
        cout << "* Maximum" << endl;
    }
    else if (s1 == "determine"){
        cout << BREAK;
        cout << "* Affected" << endl;
    }
    else if (s1 == "change"){
        cout << BREAK;
        cout << "* Data" << endl;
        cout << "* Railway" << endl;
    }
    else if (s1 == "quit" || s1 == "die"){
        goto e2;
    }
    else { // error
        process_command(s1, s2, s3);
        goto b2;
    }

    cout << endl;

    std::cin >> s2; Utils::lowercase(s2);

    if (s2 == "all" || s2 == "affected"){
        cout << BREAK;
        cout << "* Stations" << endl;
    }
    else if (s2 == "data"){
        cout << BREAK;
        cout << "* Directory" << endl;
    }
    else if (s2 == "maximum" || s2 == "max"){
        cout << BREAK;
        cout << "* Trains" << endl;
    }
    else if (s2 == "busiest"){
        cout << BREAK;
        cout << "* Stations" << endl;
        cout << "* Districts" << endl;
        cout << "* Municipalities" << endl;
        cout << "* Pairs" << endl;
    }
    else if (s2 == "railway"){
        cout << BREAK;
        if (s1 == "change") cout << "* Network" << endl;
        cout << "* Sources" << endl;
        cout << "* Sinks" << endl;
    }
    else if (s2 == "quit" || s2 == "die"){
        goto e2;
    }
    else{ // error
        process_command(s1, s2, s3);
        goto b2;
    }

    cout << endl;
    std::cin >> s3; Utils::lowercase(s3);

    if (s3 == "quit" || s3 == "die"){
        goto e2;
    }

    if (!process_command(s1, s2, s3)){
        goto b2;
    }

    cout << BREAK;
    cout << "Anything else?" << YES_NO << endl << endl;

    s1.clear(); getline(std::cin >> std::ws, s1);
    Utils::lowercase(s1);

    s_.clear(); s_.str(s1);

    while (s_ >> s1){
        if (s1 == "yes" || s1 == "y")
            goto b2;
    }

    cout << BREAK;
e2: cout << "See you next time!" << endl << endl;
}

/**
 * @brief processes the commands that were inputted
 * @param s1 first word of the command
 * @param s2 second word of the command
 * @param s3 third word of the command
 * @return 'true' if the command exists, 'false' otherwise
 */
bool Helpy::process_command(string& s1, string& s2, string& s3){
    switch (command[s1] + target[s2] + what[s3]){
        case (24) : {
            displayDataDirectory();
            break;
        }
        case (26) : {
            changeDataDirectory();
            break;
        }
        case (28) : {
            displayAllStations();
            break;
        }
        case (30) : {
            calculateMaximumTrains();
            break;
        }
        case (35) : {
            determineAffectedStations();
            break;
        }
        case (38) : {
            displayBusiest(s3);
            break;
        }
        case (41) : {
            displayBusiestPairs();
            break;
        }
        case (47) : {
            displayRailwaySources();
            break;
        }
        case (49) : {
            changeRailwaySources();
            break;
        }
        case (50) : {
            displayRailwaySinks();
            break;
        }
        case (52) : {
            changeRailwaySinks();
            break;
        }
        case (55) : {
            changeRailwayNetwork();
            break;
        }
        default : {
            cout << BREAK;
            cout << RED << "Invalid command! Please, type another command." << RESET << endl;

            return false;
        }
    }

    return true;
}

/**
 * @brief allows the user to choose the mode of the UI
 */
void Helpy::terminal(){
    string instruction = "Which mode would you prefer?\n\n"
                         "* Guided\n"
                         "* Advanced";
    uSet<string> options = {"guided", "advanced", "adv"};

    (readInput(instruction, options) == "guided") ? guided_mode() : advanced_mode();
}
