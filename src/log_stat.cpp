#include "log_stat.h"
#include <iostream>
#include <algorithm>  
#include <queue>
#include <unordered_map>
 
LogStat::LogStat(int statInterval) 
:d_interval(statInterval), d_lastEndTime(UNSET)
{}

void LogStat::generateStats(const multimap<long, Log>& logs,
							vector<Interval>& stats)
{
	stats.clear();
	if(d_lastEndTime == UNSET && !logs.empty())
	{
		d_lastEndTime = logs.begin()->second.date;
	}
	else if(logs.empty())
	{
		cout << "unexpected error" << endl;
	}

	long timeDiff = logs.rbegin()->second.date - d_lastEndTime;
	if(timeDiff >= d_interval)
	{
		// there could be multiple 10second(s) if the incoming
		// log is much later than logs
		while(timeDiff >= d_interval)
		{
			long endTime = d_lastEndTime + d_interval;
			Interval v;
			v.startDate = d_lastEndTime;
			v.endDate = endTime;
			vector<Aggregate> stat;
			calculateStat(d_lastEndTime, endTime, logs, stat);
			v.aggregates = stat;
		    stats.push_back(v);	

			d_lastEndTime = endTime + 1;
			timeDiff = logs.rbegin()->second.date - d_lastEndTime;
		}
	}
}

void LogStat::calculateStat(long start,
						    long end,
							const multimap<long, Log>& logs,
						    vector<Aggregate>& stat)
{
	cout << start << ":" << end << endl;

	unordered_map<string, int> sectionAndCounts;

	auto beginIter = logs.lower_bound(start);
	auto endIter = logs.upper_bound(end);

	for(auto itr = beginIter; itr != endIter; ++itr)
	{
		cout << "itr: " << itr->first << " : " 
						<< itr->second.date << endl;
		sectionAndCounts[itr->second.section]++;
	}

	// nlogK for aggregation
	priority_queue<Aggregate ,vector<Aggregate>, greater<Aggregate>> pq;
	for(auto& x: sectionAndCounts) 
	{
		pq.emplace(x.first, x.second);
		while(pq.size() > TOP_K)
			pq.pop();
	}

	vector<Aggregate> result;
	while(!pq.empty())
	{
		result.emplace_back(pq.top());
		pq.pop();
	}

	// KlogK sort
	sort(result.begin(), result.end(), [&](Aggregate a, Aggregate b) { 
		if(a.count != b.count) 
        	return a.count > b.count;   
		else
			return a.section < b.section; //So this is defined and testable
    });

	stat = result;
}
