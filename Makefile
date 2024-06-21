.PHONY: all

all: 
	g++ -std=c++20  -o runthread main.cpp LogSink/logsink.cpp -lsqlite3 -lwiringPi -pthread -I"LogSink"
	./runthread
test:
	g++ -std=c++20  -o test database_testing.cpp LogSink/logsink.cpp -lsqlite3 -lgtest -lgtest_main -lwiringPi -pthread -I"LogSink"

clean:
	rm -f runthread
	rm -f test
	rm -f logsink.txt
	rm -f logger.db
	
