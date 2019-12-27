#include "log_manager.h"
#include <iostream>
#include <fstream>
#include <sstream>

void readLog(const string& logFileName)
{
    bool headerRead = false;

    int statInterval = 10;
    int alertRequestPerSecond = 10;
    LogManager logManager(statInterval, alertRequestPerSecond); 

    ifstream source;
    source.open(logFileName);
    string line;
    while (getline(source, line))
    {
        if(headerRead == false) 
        {
            headerRead = true;
            continue;
        }
        stringstream ss(line);
        string t;
        vector<string> tokens;
        while (getline(ss, t, ',')) {
            tokens.push_back(t);
        }

        if(tokens.size() != 7) 
        {
            cerr << "size mismatch: " << ss.str() << endl;
            return;
        }
        else
        {
            Log log(tokens[0], tokens[1], tokens[2], stol(tokens[3]), tokens[4], stoi(tokens[5]), stoi(tokens[6]));

            vector<Interval> stats;
            bool alertIsTriggered;
            Alert alert;

            logManager.receiveLog(log, stats, alertIsTriggered, alert);
        }
    }
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std << "Usage: " << argv[0] 
            << " log.txt" << endl;
        std << "Reading from standard input..." << endl;
    }
    else
    {
        string logFileName(argv[1]);
        readLog(logFileName);
    }
    return 0;
}
