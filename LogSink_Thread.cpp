/*
//Includes...
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>
#include <poll.h>
#include <string>
#include <mutex>
#include <thread>
#include <chrono>
#include <condition_variable>  
#include <semaphore>
#include <queue>

using namespace std;

//Global Var...
const char Device_Port[]= "/dev/ttyS0";
const int RX_PIN = 10;
const int BAUD_RATE = 115200;
const string OUTPUT_FILE="logsink.txt";
const int serialPort = serialOpen(Device_Port,BAUD_RATE);
ofstream LogSink(OUTPUT_FILE);
string buffer = "";
queue <string> buff;
bool exitcall=0;
mutex locker;
condition_variable nodata;

counting_semaphore semaphore(0);


//Functions...
bool Setup(){

    if(wiringPiSetup() == -1){
        cout << "Could not complete wiringPiSetup. Errno :: "<< errno << endl;
        exit(-1);
    }
    if(serialPort == -1){
        cout << "Could not open serial port. Errno :: "<< errno << endl;
        exit(-1);
    }

    cout<<"Setup Passed"<<endl;
    return true;
}

void ReadPort(pollfd UARTTrigger){
    int counter =0;
    while(!exitcall){
        int ret = poll(&UARTTrigger, 1, -1);
        if(ret == 1){

            counter = serialDataAvail(serialPort);
            if(counter < 0){
                printf("Couldn't Read Data, Errno %d\n", errno);
                exit(-1);
            }
            else if(counter>=0){
                for(int i = 0; i<counter; i++){
                    char character= serialGetchar(serialPort);
                    buffer += character;
                    if(character == '\n'){
                        locker.lock();
                        buff.push(buffer);
                        locker.unlock();
                        buffer = "";
                        semaphore.release();
                    }
                }
    
            }
        }
    }

   semaphore.release(); 
}

void WriteFile(){
    while(!exitcall){
        semaphore.acquire();
        
        if(!buff.empty()){
            locker.lock();
            string temp = buff.front();
            buff.pop();
            locker.unlock();
            LogSink<<temp;
        }
    }
}

void cleanUp(){
    LogSink.close();
    serialClose(serialPort);
}

int main(){
    Setup();
    
    struct pollfd UARTtrigger;
    UARTtrigger.fd = serialPort;
    UARTtrigger.events = POLLIN;
    

    thread ReadUART(ReadPort, UARTtrigger);
    thread WriteOUT(WriteFile);
    printf("Threads Started...\n\n");

    while(millis()<10000){}
    exitcall = true;
    ReadUART.join();
    WriteOUT.join();

    cleanUp();
    exit(0);
}
*/
#include "logsink.h"
#include <iostream>
#include <unistd.h>

int main(){
    if(wiringPiSetup() == -1){
        std::cout << "Could not complete wiringPiSetup. Errno :: "<< errno << std::endl;
        exit(-1);
   }

    std::cout<<"Setup Passed"<<std::endl;
    
    LogSink testLogSink("/dev/ttyS0", 115200, "logsink.txt");
    testLogSink.start();
    sleep(60);
    testLogSink.stop();
    return 0;
}