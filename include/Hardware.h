#ifndef HARDWARE_H
#define HARDWARE_H
#include<json.h>
#include<string>
extern "C" {
    # include <lua.h>
    # include <lauxlib.h>
    # include <lualib.h>
}

#include <LuaBridge.h>
#include<DataVector.h>
#include<Device.h>

using namespace std;
using namespace luabridge;
class Hardware
{
    public:
        Hardware();
        virtual ~Hardware();

        void fillAsExistingHardware(unsigned int id);//add existing to program
        void fillAsNewHardware(string bus, string name, string initialScript, string initialData);//add new to program and base

        void initiateScript();

        void setId(unsigned int id);
        unsigned int getId();

        void setBus(string bus);
        string getBus();

        void setAddress(string addr);
        string getAddress();
        uint8_t getAddressAsI2C();
        uint64_t getAddressAs1W();

        void setName(string name);
        string getName();

        void setInitialScript(string initialScript);//path to initial scritp
        string getInitialScript();

        void setInitialData(string initialData);//path to data file
        string getInitialData();

        void addDevice(Device d);
        Device* getDeviceById(unsigned int id);
        Device* getDeviceByName(string name);
        Device* getDeviceByNameInScript(string name);

        Device* getDeviceByVectorId(unsigned int id);
        unsigned int getDeviceVectorSize();

        void setInitializedData();

        void create();
        void init();
        void loop();

        DataVector* getData();
        lua_State* getLuaState();

        bool doFile();
        void loadScript();

        std::shared_ptr<luabridge::LuaRef> loopRef;
        /*
        void sendByteToBus(int data);
        void sendByteToRegister(int reg, int data);
        void sendMultipleBytesToRegister(int reg, vector<int>data, unsigned int amount);
        int getByteFromBus();
        int getByteFromRegister(int reg);
        vector<int> getMultipleBytesFromRegister(int reg, unsigned int amount);
        */
        uint8_t pin = 0;

    protected:
    private:
        unsigned int id;

        DataVector data;

        lua_State* luaScript;

        std::shared_ptr<luabridge::LuaRef> createRef;
        std::shared_ptr<luabridge::LuaRef> initRef;
       // std::shared_ptr<luabridge::LuaRef> loopRef;


        vector<Device> devices; //all added devices
};

#endif // HARDWARE_H
