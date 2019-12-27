#include "log_alert.h"
#include <iostream>


LogAlert::LogAlert(int requestPerSecond)
:d_totalLogThreshold(requestPerSecond 
        * WINDOW_IN_MINUTES * SECOND_IN_MINUTE),
d_alertIsSet(false),
d_currentLogItr(),
d_logCount(-1)
{}

bool LogAlert::generateAlert(const multimap<long, Log>& logs,
                             Alert& alert)
{
    if(d_logCount == -1)
    {
        // Calling this function the first time
        d_logCount = 1;
        d_currentLogItr = logs.begin();
        return false; 
    }

    ++d_logCount;
    long mostRecentLogDate = logs.rbegin()->second.date;
    long windowStartsAt = mostRecentLogDate -
                     WINDOW_IN_MINUTES * SECOND_IN_MINUTE + 1;

    // keep track of a moving window and the count
    while(d_currentLogItr != logs.end() &&
            d_currentLogItr->first < windowStartsAt)
    {
        --d_logCount;
        d_currentLogItr++;
    }

/*
    cout << "windowStartsAt: " << windowStartsAt << " <---> " << " mostRecentLogDate: " << mostRecentLogDate << endl;
    cout << "d_logCount: " << d_logCount << endl;*/

    if(!d_alertIsSet && d_logCount > d_totalLogThreshold)
    {
        alert.state = Alert::WARNING;
        alert.when = mostRecentLogDate;
        alert.logCount = d_logCount;
        d_alertIsSet = true;
        return true;
    }
    else if(d_alertIsSet && d_logCount <= d_totalLogThreshold)
    {
        alert.state = Alert::RECOVERED;
        alert.when = mostRecentLogDate;
        alert.logCount = d_logCount;
        d_alertIsSet = false;
        return true;
    }

 //   cout << "nothing happen" << endl;
    return false;
}
