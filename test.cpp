#include "logsink.h"
#include <iostream>
#include <unistd.h>
#include <gtest/gtest.h>

class LogIntegrationTest : public ::testing::Test {
protected:
    LogSink instance = LogSink("/dev/ttyS0", 115200, "logsink.txt");

    void SetUp() override{
       //setup code outside of start()

    }

    void TearDown() override{
        //cleanup code outside of stop()

    }

}

TEST_F(LogIntegrationTest, ServiceRoutine) {
    //Step One, starting
    start();
    ASSERT_TRUE(instance.m_isRunning == true);

}