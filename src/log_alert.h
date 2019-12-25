#ifndef LOG_ALERT_H
#define LOG_ALERT_H

#include <map>
#include "util.h"
#include "log_alert.h"

using namespace std;

static const int WINDOW_IN_MINUTES = 2;
static const int SECOND_IN_MINUTE = 60;

class LogAlert
{
    public:
        LogAlert(int requestPerSecond);

        // If return is 'True', alert is modified
        // else alert is not modified
        bool generateAlert(const multimap<long, Log>& logs,
                           Alert& alert);
    private:
        int d_requestPerSecond;
        bool d_alertIsSet;
};

#endif
