How to compile and run:
```
cd src
g++ *.cpp
./a.out ../sample_csv.txt | less
```

How to compile and run unit test:
```
cd test
g++ main.cpp -I../src/ ../src/log_stat.cpp ../src/util.cpp ../src/log_manager.cpp ../src/log_alert.cpp
./a.out | less
```
