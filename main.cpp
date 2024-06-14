#include "logsink.h"
#include <iostream>
#include <unistd.h>
#include <ctime>


int main(){

    if(wiringPiSetup() == -1){
        std::cout << "Could not complete wiringPiSetup. Errno :: "<< errno << std::endl;
        exit(-1);
   }

    std::cout<<"Setup Passed"<<std::endl;
    LogSink testLogSink("/dev/ttyS0", 115200, "logsink.txt");


    testLogSink.start();
    sleep(600);
    while(millis()<600000){
        if(millis()%30000 ==0 ){
            printf("Elapsed Time: %d\n", millis());
        }
    }
    testLogSink.stop();
   

    return 0;
}