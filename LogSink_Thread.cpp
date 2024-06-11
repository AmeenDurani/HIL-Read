//Includes...
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>
#include <poll.h>
#include <queue>
#include <string>
#include <mutex>
#include <thread>
#include<chrono>

using namespace std;

//Global Var...
const char Device_Port[]= "/dev/ttyS0";
const int RX_PIN = 10;
const int BAUD_RATE = 115200;
const string OUTPUT_FILE="logsink.txt";
const int serialPort = serialOpen(Device_Port,BAUD_RATE);
ofstream LogSink(OUTPUT_FILE);
string buffer = "";
mutex key;
bool exitcall=0;

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

void ReadPort(struct pollfd UARTtrigger){
    int check, ret;

    while(!exitcall){
        ret = poll(&UARTtrigger, 1, -1);

        if(ret == 1){
            check = serialDataAvail(serialPort);
            if(check>=1 && key.try_lock()){
                printf("Reading Port\n");
                buffer += serialGetchar(serialPort);
                key.unlock();
            }
            else if(check == -1){
                cout<<"Can't Read Port! Errno:: "<<errno;
                exit(-1);
            }
        }
    }
}

void WriteFile(){
    while(!exitcall){
        if(key.try_lock()){
            printf("Writing File\n");
            LogSink<< buffer;
            buffer = "";
            key.unlock();
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
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

    while(millis()<30000){}
    exitcall = true;
    ReadUART.join();
    WriteOUT.join();

    cleanUp();
    exit(0);
}