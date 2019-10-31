#include<AutoHomePi.h>
#include <bcm2835.h>
#include<I2C.h>

#include <sched.h>
#include <sys/mman.h>


void writeRegister(uint8_t i2cAddr, int regAddress, uint8_t data) {
	char buffer[] = {regAddress, data};

	bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress(i2cAddr);
	bcm2835_i2c_write(buffer, sizeof(buffer));
    bcm2835_i2c_end();
}
int main() {
    struct sched_param sp;
    memset(&sp, 0, sizeof(sp));
    sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
    sched_setscheduler(0, SCHED_FIFO, &sp);
    mlockall(MCL_CURRENT | MCL_FUTURE);

    if (!bcm2835_init())
    {
        cout<<"Cannot initialize bcm2835"<<endl;
    }else
    {
        AutoHomePi* app = new AutoHomePi();
        app->startThreads();
        //app->addDevice(Device("Nazwa",20,"On","Opis","typ","script",12));
        app->waitForThreadsStop();
        delete app;
    }

    return 0;

}
/*
uint64_t* serial = new uint64_t[128];
    int c = oneWireScan(RPI_GPIO_P1_07,serial);
    cout<<c<<endl;
    if(c>0)
    {
        cout<<serial[0]<<endl;
        cout<<getDeviceTemperature(RPI_GPIO_P1_07,serial[0])<<endl;
    }
*/
/*
// main.cpp
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}
#include <LuaBridge.h>
#include <iostream>


using namespace luabridge;

void printMessage(const std::string& s) {
    std::cout << s << std::endl;
}

int main() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    getGlobalNamespace(L).addFunction("printMessage", printMessage);
    luaL_dofile(L, "luascript.lua");
    lua_pcall(L, 0, 0, 0);
    LuaRef sumNumbers = getGlobal(L, "sumNumbers");
    while(1)
    {
    int result = sumNumbers(5, 4);
    std::cout << "Result:" << result << std::endl;
    }

    system("pause");
}

*/
