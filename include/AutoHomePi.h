#ifndef AUTOHOMEPI_H
#define AUTOHOMEPI_H
#include <sqlite3.h>
#include<Device.h>
#include<HardwareList.h>

#include<Hardware.h>
#include<vector>
#include<string>
#include<iostream>
#include <thread>
#include <chrono>
#include <DataVector.h>
#include <OneWire.h>
#include <I2C.h>

using namespace std;

class AutoHomePi
{
    public:
        AutoHomePi();
        //added because thread is not copyable so you have to select which field you want copy
        AutoHomePi(const AutoHomePi&);
        AutoHomePi& operator=(const AutoHomePi&){cout<<"Druga opcja"<<endl;};
        //
        virtual ~AutoHomePi();

        //sqlite operation wrapper
        static vector<vector<string>> sqlGetData(string sql);
        static void sqlExecute(string sql);





        void test();

        void startThreads();
        void waitForThreadsStop();

        int getSystem();

    protected:
    private:
        //threads
        std::thread luaThread;
        std::thread httpThread;
        std::thread websocketThread;

        //thread function
        void luaInterpreter();
        void httpServer();
        void webSocketServer();

        static sqlite3 *db;

        HardwareList* hlist;

        bool running = true;
};

#endif // AUTOHOMEPI_H
