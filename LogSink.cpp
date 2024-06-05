#include <cstdlib>
#include <fstream>
#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>
#include <poll.h>
#include <queue>
#include <string>
using namespace std;

#define DEVICE_PORT "/dev/ttyS0"
#define RX_PIN 10
#define BAUD_RATE 115200
#define OUTPUT_FILE "logsink.txt"
int serialPort = serialOpen(DEVICE_PORT,BAUD_RATE);
ofstream LogSink(OUTPUT_FILE);

int main(){
    if(wiringPiSetup() == -1){
        cout << "Could not complete wiringPiSetup. Errno :: "<< errno << endl;
        exit(-1);
    }
    if(serialPort == -1){
        cout << "Could not open serial port. Errno :: "<< errno << endl;
        exit(-1);
    }
    
    struct pollfd mypoll;
    mypoll.fd = serialPort;
    mypoll.events = POLLIN;
    int counter = 0;
    bool toggle =0;
    string buffer;
    char thing;
    queue<char> buff;
    int tim_start = millis();

    while(1){
        
        int ret = poll(&mypoll, 1, 200);
        if(ret == 1){
           counter = serialDataAvail(serialPort);
           for(int i = 0; i<counter; i++){
            thing = serialGetchar(serialPort);
            buffer += thing;
            if(thing == '\n')
                counter = 0;
           }
        }
        else if(ret == -1){
            cout<< "Error. Errno :: " << errno<<endl;
        }
        else{
            
           if(thing == '\n'){
            cout<<"{ "<<millis()-tim_start<<" ms} "<<buffer<<"wait...\n";
            buffer = "";
            thing = '.';
           }

        }

    }
    
    serialClose(serialPort);
    exit(0);
}