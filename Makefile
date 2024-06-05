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