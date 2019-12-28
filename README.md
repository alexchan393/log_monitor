## What do I need to run this program: 
unix environment with g++ installed

## How to compile and run:
```
cd src
g++ *.cpp
./a.out ../sample_csv.txt | less
```

## How to compile and run unit test:
```
cd test
g++ main.cpp -I../src/ ../src/log_stat.cpp ../src/util.cpp ../src/log_manager.cpp ../src/log_alert.cpp
./a.out | less
```

## Few important points about the program and design:
1- For the 10 seconds statistic calculation, we perform aggregation using a hashtable and heap if we hit the 10 seconds mark. (Mostly dictated by the heap (NlogK) but negligible since this is every 10 seconds)

2- For the alert, I kept track of a moving window. (For N number of log, it will just be O(N))

3- I made an assumption that the logs are mostly sorted, there could be few logs that are out-of-order (I observed this from the sample input file), therefore I used a multimap to store the logs, such that it can handle out-of-order inputs and generate more accurate statistics and alert.

4- If the incoming log has the correct timestamp (later than the curret most recent one), then insertion to the multimap is constant, otherwise is O(logN).

5- I purge the data when it gets old enough to keep the memory footprint low.


## Improvement:
1- Add more unit test for the out-of-order use cases once the requirement is confirmed.

2- Add MakeFile/CICD related tools. (Jenkins, Valgrind, etc)

3- Add GTEST instead of the assert() style in the test folder.

4- Support for Max/Min/STD, etc for statistic.

5- Storage for the statistic and alert history.

6- Alert could have follow up actions.

7- Add a web base UI for this tool.
