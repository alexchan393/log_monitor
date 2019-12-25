#include "log_manager.h"
#include <assert.h>
#include <iostream>

void TEST_STAT()
{
    bool notCareAlert;
    Alert notCareObj;
    
    LogManager logManager(10,1);
    Log log;
    log.date = 2;
    log.section = "abc";
    vector<Interval> expectedResults;
    logManager.receiveLog(log, expectedResults, notCareAlert, notCareObj);
    assert(expectedResults.empty());

    log.date = 2;
    log.section = "jk";
    logManager.receiveLog(log, expectedResults, notCareAlert, notCareObj);
    assert(expectedResults.empty());

    log.date = 5;
    log.section = "abc";
    logManager.receiveLog(log, expectedResults, notCareAlert, notCareObj);
    assert(expectedResults.empty());

    log.date = 5;
    log.section = "jk";
    logManager.receiveLog(log, expectedResults, notCareAlert, notCareObj);
    assert(expectedResults.empty());

    log.date = 9;
    log.section = "abc";
    logManager.receiveLog(log, expectedResults, notCareAlert, notCareObj);
    assert(expectedResults.empty());

    log.date = 10;
    log.section = "xyz";
    logManager.receiveLog(log, expectedResults, notCareAlert, notCareObj);
    assert(expectedResults.empty());

    log.date = 10;
    log.section = "ignored1";
    logManager.receiveLog(log, expectedResults, notCareAlert, notCareObj);
    assert(expectedResults.empty());

    log.date = 10;
    log.section = "ignored2";
    logManager.receiveLog(log, expectedResults, notCareAlert, notCareObj);
    assert(expectedResults.empty());

    log.date = 12;
    log.section = "xyz";
    logManager.receiveLog(log, expectedResults, notCareAlert, notCareObj);
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
    logManager.receiveLog(log, expectedResults, notCareAlert, notCareObj);
    assert(expectedResults.size() == 1);
    assert(expectedResults[0].startDate == 13);
    assert(expectedResults[0].endDate == 23);
    assert(expectedResults[0].aggregates.empty());
    assert(logManager.getSize() == 10);

    log.date = 67;
    log.section = "efg";
    logManager.receiveLog(log, expectedResults, notCareAlert, notCareObj);
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

    assert(logManager.getSize() == 11);
    cout << "STAT PASSED" << endl;
}

void TEST_ALERT()
{
    bool alertIsTriggered; 
    Alert alert;
    
    long START = 0;
    // 1(log per second), this setup triggers alert at the threshold of 120 logs in 2 mins
    LogManager logManager(10, 1);
    Log log;
    log.date = START;
    log.section = "abc";
    vector<Interval> notCareObj;

    int count = 140;
    while(count != 0)
    {
        log.date += 1; // let say 1 request every second
        log.section = "abc";
        vector<Interval> notCareObj;
        logManager.receiveLog(log, notCareObj, alertIsTriggered, alert);

        if(log.date - START == 121)  // then a warning alert triggered at 121 second
        {
            assert(alertIsTriggered == true);
            assert(alert.state == Alert::WARNING);
            assert(alert.when == 121);
        }
        else
            assert(alertIsTriggered == false);

        --count;
    }
    assert(logManager.getSize() == 140);

    count = 10;
    while(count != 0)
    {
        log.date += 2; // now 1 request every 2 second
        log.section = "abc";
        logManager.receiveLog(log, notCareObj, alertIsTriggered, alert);

        if(count == 10) // then an recovery alert triggered immediately
        {
            assert(alertIsTriggered == true);
            assert(alert.state == Alert::RECOVERED);
            assert(alert.when == 142);
        }
        else
            assert(alertIsTriggered == false);

        --count;
    }

    assert(logManager.getSize() == 150);

    count = 20;
    while(count != 0)
    {
        //now keep log.date; unchanged, to minic all the logs come at the same time
        log.section = "qwe";
        logManager.receiveLog(log, notCareObj, alertIsTriggered, alert);

        cout << "count: " << count << endl;
        if(count == 11) // then a warning alert triggered after it reached the 11th log
        {
            assert(alertIsTriggered == true);
            assert(alert.state == Alert::WARNING);
            assert(alert.when == 160);
        }
        else
            assert(alertIsTriggered == false);

        --count;
    }

    // now there is a log comes in a much later time
    log.date += 1000;
    log.section = "qwe";
    logManager.receiveLog(log, notCareObj, alertIsTriggered, alert);
    assert(alertIsTriggered == true);
    assert(alert.state == Alert::RECOVERED);
    assert(alert.when == 1160);

    // All previous logs were purged because they were 120 seconds older than the most recent one (1160)
    assert(logManager.getSize() == 1);

    cout << "ALERT PASSED" << endl;
}

int main()
{
    TEST_STAT();
    TEST_ALERT();
    return 0;
}
