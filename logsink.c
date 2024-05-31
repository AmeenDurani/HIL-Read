#include <stdio.h>
#include <errno.h>
#include <wiringSerial.h>
#include <wiringPi.h>

int main(void){

  // Setup Code
  int handle = serialOpen("/dev/ttyS0",115200); 
  if(handle == -1){
    printf("Serial Setup Failed :: %d", errno);
    exit(-1);
  }
  char buffer[256] = {0};
  int num = 0;
  int index = 0;
  printf("Setup Complete...\n");

  // Logsink
  while(1){
    num = serialDataAvail(handle);
    if(num > 0){
      for(int i = index; i<num; i++){
        buffer[i] = (char)serialGetChar(handle);
      }
      index += num;
      if(buffer[index - 1] == '\n'){
        buffer[255] = "\0";
        printf("%s\n", buffer);
        memset(buffer, 0, sizeof(buffer));
      }
      
    }
    else if(num < 0){
      printf("Data Detch Error :: %d\n", errno);
      exit(-1);
    }
    
  }
  // Cleanup
  serialClose(handle);
  exit(0);
}
