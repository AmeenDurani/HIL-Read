#include "logsink.h"
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <fstream>
#include <string>

#define minLogsAquired 7
#define specificLog "Firmware/OBC-firmware/obc/app/drivers/sdcard/sdc_bdev.c:416 - 108"
#define logFile "logsink.txt"
#define timeperiod 10



int main(){

    LogSink logger("/dev/ttyS0", 115200, "logsink.txt");
    logger.runFor(15);

    return 0;
}