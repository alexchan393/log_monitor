#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

using namespace std;

struct Alert
{
    enum State
    {
        WARNING,
        RECOVERED
    };
    State state;
    long when;
};

struct Log
{
	string remoteHost;
	string rfc931;
	string authuser;
	long   date;
	string section;
	int status;
	int bytes;
    Log()
    {}
    Log(string a, string b, string c, long l, string d, int i, int i2)
        :remoteHost(a),
         rfc931(b),
         authuser(c),
         date(l),
         section(d),
         status(i),
         bytes(i2)
    {}
};

struct Aggregate
{
	string section;
	int count;
	Aggregate(string s, int c):
		section(s), count(c)
	{}
};

struct Interval
{
	long startDate;
	long endDate;
	vector<Aggregate> aggregates;
};


bool operator > (const Aggregate& lhs, const Aggregate& rhs);

#endif
