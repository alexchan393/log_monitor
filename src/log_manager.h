#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <map>
#include "util.h"
#include "log_stat.h"
#include "log_alert.h"

using namespace std; 

class LogManager
{
    public:
        LogManager(int statInterval, 
                   int alertAveragePerSecond);
        void receiveLog(const Log& log,
                        vector<Interval>& stats,
                        bool& alertTriggered,
                        Alert& alert);
        int getSize(); // for unit test purpose

    private:
        multimap<long, Log> d_logs;
        LogStat d_logStat;
        LogAlert d_logAlert;
        int d_timeToLive;

    private:
        void purgeData();
};

#endif
