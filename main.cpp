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

    // Did it read expected times?
    std::ifstream read;
    read.open(logFile);
    if(!read.is_open()){
        exit(-1);
    }
    char buff[256] ={'\0'};
    int count=0;
    while(read.getline(buff,256)){
        std::cout<<buff<<std::endl;
        count++;
    }
    std::cout<< (count>7) << " Required: "<<7<<" Iterated: "<<count<<std::endl;
    read.close();



    // Did it read a specific log?
    read.open(logFile);
    char buffarr[256]={'\0'};
    std::string buffstr;
    count=0;
    bool found = false;
    while(read.getline(buffarr,256)){
        buffstr = buffarr;
        
        if(buffstr.find(specificLog) != std::string::npos){
            found = true;
            break;
        }
        count++;
    }
   std::cout<<found <<" Iterated "<<count<<" times!\n";
    read.close();
   

    return 0;
}