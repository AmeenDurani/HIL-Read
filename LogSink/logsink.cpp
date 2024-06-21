#include "logsink.h"
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <poll.h>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <sqlite3.h>

LogSink::LogSink(std::string uartPort, int baudRate, std::string outputFile){
    m_outputFile.open(outputFile);
    if(!m_outputFile.is_open()){
        std::cerr<<"Could Not Open File. Errno "<<errno<<std::endl;
        return;
    }
    m_serialFd = serialOpen(uartPort.c_str(), baudRate);
    if(m_serialFd == -1){
        std::cerr<<"Could not open UART Port. Errno "<<errno<<std::endl;
        return;
    }
    m_isRunning = false;

}
LogSink::~LogSink(){
    if(m_isRunning){
        stop();
        m_readThread.join();
        m_writeThread.join();
    }
    serialClose(m_serialFd);
    m_outputFile.close();
}
void LogSink::uartReadThread(){

    std::string buffer = "";
    struct pollfd uartWait;
    uartWait.fd = m_serialFd;
    uartWait.events = POLLIN;

    // time_t rawtime;
    // struct tm * timeinfo;
    // char timer [80];
    // std::string times;


    while(m_isRunning){
        int ret = poll(&uartWait, 1, -1);
        if(ret == 1){

            int bytesAvailable = serialDataAvail(m_serialFd);
            if(bytesAvailable < 0){
                printf("Couldn't Read Data, Errno %d\n", errno);
                exit(-1);
            }
            else if(bytesAvailable>=0){
                for(int i = 0; i<bytesAvailable; i++){
                    char character= serialGetchar(m_serialFd);
                    buffer += character;
                    if(character == '\n'){
                        // time(&rawtime);
                        // timeinfo = localtime(&rawtime);
                        // strftime (timer, 80, "[%r]", timeinfo);    
                        // times = timer;
                        //buffer = times+" "+buffer;

                        m_queueLock.lock();
                        //m_logQueue.push(times);
                        m_logQueue.push(buffer);
                        m_queueLock.unlock();
                        buffer = "";
                        m_queueSemaphore.release();
                    }
                }
    
            }
        }
    }

   m_queueSemaphore.release(); 
}
void LogSink::writeFileThread(){
    sqlite3* db;
    const char* sql_start=  "CREATE TABLE IF NOT EXISTS LOGTABLE ("
                            "NO INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "TIMESTAMP TEXT NOT NULL DEFAULT (time('now', 'localtime')), "
                            "LOG TEXT);";
    const char* sql_append = "INSERT INTO LOGTABLE (LOG) VALUES (?);";
    const char* sql_outfile = "SELECT * FROM LOGTABLE;";
    int rc;
    char* errMsg = nullptr;
    sqlite3_stmt* stmt;
    const char* sql_log;

    rc = sqlite3_open("logger.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        exit(-1);
    } else {
        std::cout << "DB opened successfully" << std::endl;
    }

    rc = sqlite3_exec(db, sql_start, nullptr, nullptr,&errMsg);
    if(rc != SQLITE_OK){
        std::cerr<<"FAILED TABLE CREATION: "<< errMsg <<std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        exit(-1);
    } else{
        std::cout<<"Table Creation Success\n";
    }

    while(m_isRunning){
        m_queueSemaphore.acquire();
        
        if(!m_logQueue.empty()){

            rc = sqlite3_prepare_v2(db, sql_append, -1, &stmt, nullptr);

            m_queueLock.lock();
            sql_log = (m_logQueue.front()).c_str();
            m_logQueue.pop();
            m_queueLock.unlock();

            if(rc == SQLITE_OK){
                printf("Log -> %s\n", sql_log);
                sqlite3_bind_text(stmt, 1, sql_log, -1, SQLITE_TRANSIENT);
                rc = sqlite3_step(stmt);
                if(rc != SQLITE_DONE){
                    std::cerr << "Error inserting data: "<< sqlite3_errmsg(db) <<std::endl;
                    sqlite3_close(db);
                    exit(-1);
                }
                sqlite3_finalize(stmt);
            }
            /*
            m_queueLock.lock();
            std::string temp = m_logQueue.front();
            m_logQueue.pop();
            m_queueLock.unlock();
            m_outputFile<<temp;
            */

        }

    }
    printf("writing to file\n");
    rc = sqlite3_prepare_v2(db, sql_outfile, -1, &stmt, nullptr);
    m_outputFile << "No\tTimestamp\tLog\n";
    while(sqlite3_step(stmt) == SQLITE_ROW){
        int no = sqlite3_column_int(stmt,0);
        const unsigned char* timestamp = sqlite3_column_text(stmt,1);
        const unsigned char* logg = sqlite3_column_text(stmt, 2);
        m_outputFile << no <<"\t"<<timestamp<<"\t"<<logg<<"\n";
    }
    printf("Closed db\n");
    sqlite3_close(db);
}
int LogSink::start(){
    m_isRunning = true;
    m_readThread = std::thread(&LogSink::uartReadThread, this);
    m_writeThread = std::thread(&LogSink::writeFileThread, this);

    return 0;
}
void LogSink::stop(){
    m_isRunning = false;
    m_readThread.join();
    m_writeThread.join();
    m_outputFile.close();
}

void LogSink::runFor(int seconds){
    if(wiringPiSetup() == -1){
        std::cout << "Could not complete wiringPiSetup. Errno :: "<< errno << std::endl;
        exit(-1);
   }
   this->start();
   sleep(seconds);
   this->stop();
}