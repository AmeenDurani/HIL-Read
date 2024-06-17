.PHONY: all

all: 
	g++ -std=c++20  -o runthread main.cpp LogSink/logsink.cpp -lwiringPi -pthread -I"LogSink"
	./runthread
test:
	g++ -std=c++20  -o test test.cpp LogSink/logsink.cpp -lgtest -lgtest_main -lwiringPi -pthread -I"LogSink"

clean:
	rm -f runthread
	rm -f test
	rm -f logsink.txt
