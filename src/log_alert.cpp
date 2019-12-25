#include "log_alert.h"
#include <iostream>

static const int SECOND_IN_MINUTE = 60;

LogAlert::LogAlert(int requestPerSecond)
:d_requestPerSecond(requestPerSecond),
d_alertIsSet(false)
{}

bool LogAlert::generateAlert(const multimap<long, Log>& logs,
                             Alert& alert)
{
    int totalNumRequestThreshold = 
        d_requestPerSecond * WINDOW_IN_MINUTES * SECOND_IN_MINUTE;

    long mostRecentLogDate = logs.rbegin()->second.date;
    long start = mostRecentLogDate -
                     WINDOW_IN_MINUTES * SECOND_IN_MINUTE;

    auto beginIter = logs.lower_bound(start);
    auto endIter = logs.end(); 

    int currentNumRequest = 0;
    
    for(auto itr = beginIter; itr != endIter; ++itr)
    {
        currentNumRequest += 1;
    }
    cout << "map size: " << logs.size() << endl;
    cout << "start: " << start << "---" << "mostRecentLogDate: " << mostRecentLogDate << endl;
    cout << "currentNumRequest: " << currentNumRequest << endl;
    cout << "TotalNumRequestThreshold: " << totalNumRequestThreshold << endl;

    if(!d_alertIsSet && currentNumRequest > totalNumRequestThreshold)
    {
        alert.state = Alert::WARNING;
        alert.when = mostRecentLogDate;
        d_alertIsSet = true;
        return true;
    }
    else if(d_alertIsSet && currentNumRequest <= totalNumRequestThreshold)
    {
        alert.state = Alert::RECOVERED;
        alert.when = mostRecentLogDate;
        d_alertIsSet = false;
        return true;
    }

    cout << "nothing happen" << endl;
    return false;
}
