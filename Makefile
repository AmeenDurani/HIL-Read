.PHONY: all

all: 
	g++ -std=c++20  -o runthread main.cpp LogSink/logsink.cpp -lwiringPi -pthread -I"LogSink"
	./runthread
clean:
	rm -f runthread
