#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>
#include <wiringPi.h>

int main(void){

  // Setup Code
  if(wiringPiSetup() == -1){
    printf("WiringPi innit failed :: %d", errno);
    exit(-1);
  }
  int handle = serialOpen("/dev/ttyS0",115200); 
  if(handle == -1){
    printf("Serial Setup Failed :: %d", errno);
    exit(-1);
  }
  int toggle = 1;
  int num = 0;
  char buffer = '0';
  printf("Setup Complete...\n");
  
  // Logsink
  while(1){
    num = serialDataAvail(handle);
    
    if(num>1){
        for(int i = 0; i<num; i++){
            buffer = serialGetchar(handle);
            
            printf("%c", buffer);
            if(buffer == '\n' || toggle == 1){
              printf("[Time :: %d ms] ", millis());
              toggle = 0;
            }
        }
    }
    else if(num < 0){
      printf("Data Fetch Error :: %d\n", errno);
      exit(-1);
    }
    delay(2000);
  }
  // Cleanup
  serialClose(handle);
  
  exit(0);
}