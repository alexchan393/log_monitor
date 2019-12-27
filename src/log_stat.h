#ifndef LOG_STAT_H
#define LOG_STAT_H

#include <map>
#include "util.h"
#include "log_stat.h"

using namespace std;

static const int TOP_K = 3;
static const int UNSET = -1;

class LogStat
{
    public:
        LogStat(int statInterval);
        void generateStats(const multimap<long, Log>& logs,
                           vector<Interval>& stats);
    private:
        int d_interval;

        // use this to keep track of the previous end time, so that it knows
        // when to collect the new 10 seconds information
        long d_lastEndTime;
    private:
        void calculateStat(long start,
                           long end,
                           const multimap<long, Log>& logs,
                           vector<Aggregate>& stat, 
                           int& totalCount);
};

#endif
