#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <map>
#include "util.h"
#include "log_stat.h"

using namespace std;

class LogManager
{
	public:
		LogManager(int statInterval, 
				   int alertAverage);
		void receiveLog(const Log& log,
						vector<Interval>& stats);
		int getSize(); // for unit test purpose

	private:
		multimap<long, Log> d_logs;
		LogStat d_logStat;
		int d_timeToLive;

	private:
		void purgeData();
};

#endif
