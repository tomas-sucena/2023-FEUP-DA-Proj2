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

std::map<string, int> Helpy::command = {{"change", 1}, {"run", 2}};

std::map<string, int> Helpy::target = {{"selected", 4}, {"backtracking", 6}, {"approximation", 8}, {"other", 10}};

std::map<string, int> Helpy::what = {{"graph", 12}, {"tsp", 15}};

/**
 * @brief creates a new Helpy object
 */
Helpy::Helpy() : reader("../../data", ',', '\n') {
    fetchData("Toy-Graphs", "shipping.csv");
}

/**
 * @brief reads and parses the data files
 */
void Helpy::fetchData(string type, string what) {
    // create the graph
    graph = reader.read(type, what);
    // fill the data structures
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
    cout << "* Change" << endl;
    cout << "* Run" << endl;
    cout << endl;

    string s1, s2, s3;
    std::istringstream s_;

    std::cin >> s1; Utils::lowercase(s1);

    if (s1 == "change"){
        cout << BREAK;
        cout << "* Selected" << endl;
    }
    else if (s1 == "run"){
        cout << BREAK;
        cout << "* BackTracking" << endl;
        cout << "* Approximation" << endl;
        cout << "* Other" << endl;
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

    if (s2 == "selected"){
        cout << BREAK;
        cout << "* Graph" << endl;
    }
    else if (s2 == "approximation" || s2 == "backtracking" || s2 = "other"){
        cout << BREAK;
        cout << "* TSP" << endl;
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
        case(17) : {
            changeSelectedGraph();
            break;
        }

        case(23) : {
            runBackTrackingTSP();
            break;
        }

        case(25) : {
            runApproximationTSP();
            break;
        }

        case(27) : {
            runOtherTSP();
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

void Helpy::changeSelectedGraph() {
    //perguntar ao user qual e o grafo que ele quer e chamar o fetchData
    printf("Under Development");
    return;
}

void Helpy::runApproximationTSP() {
    //algoritmos ahhhhhhh
    printf("Under Development");
    return;
}

void Helpy::runBackTrackingTSP() {
    //algoritmos ahhhhhhh
    printf("Under Development");
    return;
}

void Helpy::runOtherTSP() {
    //algoritmos ahhhhhhh
    printf("Under Development");
    return;
}
