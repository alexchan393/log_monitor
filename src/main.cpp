#include "log_manager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

string getSection(const string& input)
{
    for(int x = 0; x < input.size(); ++ x)
    {
        // first '/'
        if(input[x] == '/')
        {
            int start = x + 1;
            while(start < input.size())
            {
                // either second '/' or whitespace, whichever come first
                if(input[start] == '/' || input[start] == ' ')
                {
                    return input.substr(x, start - x);
                }
                ++start;
            }
        }
    }
    return "parsing error";
}

void printResult(const vector<Interval>& stats, bool alertIsTriggered, const Alert& alert)
{
    ////////////////////// STATISTIC //////////////////////
    if(!stats.empty())
        cout << "\n\nStatistic " << endl;
    for(int x = 0; x < stats.size(); ++ x)
    {
        time_t secsSinceEpoch = stats[x].startDate;
        cout << left << setw(10) << "start at " << put_time(localtime(&secsSinceEpoch), "%Y/%m/%d %T") << endl;

        secsSinceEpoch = stats[x].endDate;
        cout << left << setw(10) << "end at   " << put_time(localtime(&secsSinceEpoch), "%Y/%m/%d %T") << endl;
        cout << left << setw(10) << "total hit " << stats[x].totalCount << endl;
        cout << left << setw(30) << "Hit Rank " << endl;
        for(int y = 0; y < stats[x].aggregates.size(); ++ y)
        {
            cout << left << setw(30) << stats[x].aggregates[y].section << " : " << stats[x].aggregates[y].count << endl;
        }
    }


    ////////////////////// ALERT //////////////////////
    if(alertIsTriggered)
    {
        if(alert.state == Alert::WARNING)
            cout << "\n\n======= High Traffic Generated An Alert - hit = " << alert.logCount << endl;
        else
            cout << "\n\n======= Recovery Alert Triggered - hit = "<< alert.logCount << endl;

        time_t secsSinceEpoch = alert.when;
        cout << "at " << put_time(localtime(&secsSinceEpoch), "%Y/%m/%d %T") << endl;
    }
}

void readLog(const string& logFileName, int alertRequestPerSecond)
{
    bool headerRead = false;

    int statIntervalInSecond = 10;
    LogManager logManager(statIntervalInSecond, alertRequestPerSecond); 

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
            continue;
        }
        else
        {
            Log log(tokens[0], tokens[1], tokens[2], stol(tokens[3]), getSection(tokens[4]), stoi(tokens[5]), stoi(tokens[6]));

            vector<Interval> stats;
            bool alertIsTriggered;
            Alert alert;

            logManager.receiveLog(log, stats, alertIsTriggered, alert);

            printResult(stats, alertIsTriggered, alert);
        }
    }
}

int main(int argc, char* argv[]) {
    if(argc != 2 && argc != 3) {
        cout << "Usage: " << argv[0] 
             << " log.txt log_per_second_for_alert" <<
             endl;
        return -1;
    }
    
    int alertRequestPerSecond = 10;
    if(argc == 3)
        alertRequestPerSecond = atoi(argv[2]);

    string logFileName(argv[1]);
    readLog(logFileName, alertRequestPerSecond);

    return 0;
}
