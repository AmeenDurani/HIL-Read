TARGET =
.PHONY: all

all: $(TARGET)

logsink:
	rm -f logsink
	g++ -o logsink LogSink.cpp -lwiringPi
	./logsink
logsinkfor:
	rm -f logsinkfor
	gcc -o logsinkfor LogSink_For.c -lwiringPi
	./logsinkfor
logsinkmgr:
	rm -f logsinkmgr
	g++ -std=c++20  -o logsinkmgr LogSink_Thread.cpp LogSink/logsink.cpp -lwiringPi -pthread -I"LogSink"
	./logsinkmgr
clean:
	rm -f logsinkmgr
