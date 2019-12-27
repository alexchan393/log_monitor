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
1- For the 10 seconds statistic calculation, I did a binary search on the previous end time and then aggregate using a heap. (Mostly dictated by the heap (NlogK) but negligible since this is every 10 seconds)\

2- For the alert, I kept track of a moving window. (For N number of log, it will just be O(N))\

3- I made an assumption that the logs are mostly sorted, there could be few logs that are out-of-order (I observed this from the sample input file), therefore I used a multimap to keep the logs, such that it is more tolerant to out-of-order input and can generate more accurate statistics and alert results.\

4- If the incoming log has the correct timestamp, then insertion to the multimap is constant, otherwise is logN.\

5- I purge the data when it gets old enough to keep the memory footprint low.\


## Improvement:
1- Add more unit test for the out-of-order use cases once the requirement is confirmed.\

2- Add MakeFile/CICD related tools. (Jenkins, Valgrind, etc)\

3- Add GTEST instead of the assert() style setup in the test folder.\

4- Support for Max/Min/STD/2Days_high/, etc for statistic.\

5- Storage for the statistic and alert.\

6- Alert need to have follow up action to bring up the awareness.\

7- Add a web base UI for this tool.\
