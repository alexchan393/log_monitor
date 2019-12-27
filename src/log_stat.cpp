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
    if(logs.empty())
    {
        cout << "logs is empty, unexpected error" << endl; 
        return;
    }

    stats.clear();
    if(d_lastEndTime == UNSET)
    {
        if(logs.rbegin()->second.date - logs.begin()->second.date < d_interval)
            return;
        else
            d_lastEndTime = logs.begin()->second.date;
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

            calculateStat(d_lastEndTime, endTime, logs, stat, v.totalCount);

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
                            vector<Aggregate>& stat,
                            int& totalCount)
{
    totalCount = 0;
    unordered_map<string, int> sectionAndCounts;

    // both lower_bound / upper_bound do binary search hence they are logN
    // this can be further optimized if we keep a member variable for the iterator that point the previous d_lastEndTime
    auto beginIter = logs.lower_bound(start);
    auto endIter = logs.upper_bound(end);

    for(auto itr = beginIter; itr != endIter; ++itr)
    {
 //       cout << "itr: " << itr->first << endl;
        sectionAndCounts[itr->second.section]++;
        ++totalCount;
    }

    // Use a min heap so we can get NlogK for getting the top K hits section
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

    // min heap is used previously, therefore we need to reverse the result vector
    reverse(result.begin(), result.end()); 

    stat = result;
}
