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
	g++ -o logsinkmgr LogSink_Thread.cpp -lwiringPi
	./logsinkmgr