#include "log_manager.h"
#include <iostream>

LogManager::LogManager(int statInterval,
                       int alertAverage)
:d_logStat(statInterval),
d_timeToLive(statInterval*2)
{

}

void LogManager::receiveLog(const Log& log,
                            vector<Interval>& stats)
{
    // If log.date is bigger than d_logs, then insert() is constant
    // because insert() takes the 'hint' iterator which is log.end().
    // Otherwise it is logarithmic 
    d_logs.insert(d_logs.end(), {log.date, log});
    d_logStat.generateStats(d_logs, stats);
    purgeData();
}

void LogManager::purgeData()
{
    cout << "before purge size: " << d_logs.size() << endl;
    while(d_logs.begin()->second.date < (d_logs.rbegin()->second.date - d_timeToLive))
    {
        cout << "purged begin() " << d_logs.begin()->second.date
         << ", last() " << d_logs.rbegin()->second.date << endl;
        d_logs.erase(d_logs.begin());
    }
    cout << "after purge size: " << d_logs.size() << endl;
}

int LogManager::getSize()
{
    return d_logs.size();
}
