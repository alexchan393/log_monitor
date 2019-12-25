#include "log_manager.h"
#include <assert.h>
#include <iostream>

void TEST_LOGSTAT()
{
    LogManager logManager(10,1);
    Log log;
    log.date = 2;
    log.section = "abc";
    vector<Interval> expectedResults;
    logManager.receiveLog(log, expectedResults);
    assert(expectedResults.empty());

    log.date = 2;
    log.section = "jk";
    logManager.receiveLog(log, expectedResults);
    assert(expectedResults.empty());

    log.date = 5;
    log.section = "abc";
    logManager.receiveLog(log, expectedResults);
    assert(expectedResults.empty());

    log.date = 5;
    log.section = "jk";
    logManager.receiveLog(log, expectedResults);
    assert(expectedResults.empty());

    log.date = 9;
    log.section = "abc";
    logManager.receiveLog(log, expectedResults);
    assert(expectedResults.empty());

    log.date = 10;
    log.section = "xyz";
    logManager.receiveLog(log, expectedResults);
    assert(expectedResults.empty());

    log.date = 10;
    log.section = "ignored1";
    logManager.receiveLog(log, expectedResults);
    assert(expectedResults.empty());

    log.date = 10;
    log.section = "ignored2";
    logManager.receiveLog(log, expectedResults);
    assert(expectedResults.empty());

    log.date = 12;
    log.section = "xyz";
    logManager.receiveLog(log, expectedResults);
    assert(expectedResults.size() == 1);

    // In the header we hard coded to show the top 3 only
    assert(expectedResults[0].aggregates.size() == 3);
    assert(expectedResults[0].startDate == 2);
    assert(expectedResults[0].endDate == 12);
    assert(expectedResults[0].aggregates[0].section == "abc");
    assert(expectedResults[0].aggregates[0].count == 3);
    assert(expectedResults[0].aggregates[1].section == "jk");
    assert(expectedResults[0].aggregates[1].count == 2);
    assert(expectedResults[0].aggregates[2].section == "xyz");
    assert(expectedResults[0].aggregates[2].count == 2);
    assert(logManager.getSize() == 9);

    log.date = 25;
    log.section = "xyz";
    logManager.receiveLog(log, expectedResults);
    assert(expectedResults.size() == 1);
    assert(expectedResults[0].startDate == 13);
    assert(expectedResults[0].endDate == 23);
    assert(expectedResults[0].aggregates.empty());

    // before size is 10, after is 8, because date = 2 are purged
    assert(logManager.getSize() == 8);

    log.date = 67;
    log.section = "efg";
    logManager.receiveLog(log, expectedResults);
    assert(expectedResults.size() == 4);
    assert(expectedResults[0].startDate == 24);
    assert(expectedResults[0].endDate == 34);
    assert(expectedResults[0].aggregates.size() == 1);
    assert(expectedResults[0].aggregates[0].section == "xyz");
    assert(expectedResults[0].aggregates[0].count == 1);

    assert(expectedResults[1].aggregates.size() == 0);
    assert(expectedResults[1].startDate == 35);
    assert(expectedResults[1].endDate == 45);
    assert(expectedResults[2].aggregates.size() == 0);
    assert(expectedResults[2].startDate == 46);
    assert(expectedResults[2].endDate == 56);

    assert(expectedResults[3].aggregates.size() == 1);
    assert(expectedResults[3].startDate == 57);
    assert(expectedResults[3].endDate == 67);
    assert(expectedResults[3].aggregates[0].section == "efg");
    assert(expectedResults[3].aggregates[0].count == 1);

    // before size is 9, after is 1. Only date = 67 is left
    assert(logManager.getSize() == 1);
    cout << "LOGSTAT PASSED" << endl;
}
int main()
{
    TEST_LOGSTAT();
    return 0;
}
