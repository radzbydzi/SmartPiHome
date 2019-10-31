#ifndef DEVICE_H
#define DEVICE_H
extern "C" {
    # include <lua.h>
    # include <lauxlib.h>
    # include <lualib.h>
}

#include <LuaBridge.h>
#include<string>
#include <memory>
#include<json.h>
#include<DataVector.h>

using namespace luabridge;
using namespace std;

class Device
{
    public:

        Device();
        virtual~Device();

        void fillAsExistingDevice(unsigned int id);//add existing device from base to program
        void fillAsNewDevice(string name, unsigned int hardwareId, string state, string description, string type, string script, unsigned int roomId);//add new device

        void initiateScript();

        void setId(unsigned int id);
        unsigned int getId();

        void setName(string name);
        string getName();

        void setNameInScript(string name);
        string getNameInScript();

        void setHardwareId(unsigned int hardwareId);
        unsigned int getHardwareId();

        void setState(string state);
        string getState();

        void setDescription(string description);
        string getDescription();

        void setType(string type);
        string getType();

        void setScript(string script);
        string getScript();

        void setRoomId(unsigned int roomId);
        unsigned int getRoomId();

        void setStateChanged(bool b);
        bool isStateChanged();
        void setEdge(string s);
        bool isRisingEdge();
        bool isFallingEdge();

        void setHardwareScriptName(string s);
        string getHardwareScriptName();
        //bool

        bool doFile();
        void loadScript();

        void onHigh();
        void onLow();
        void onChange();
        void onRisingEdge();
        void onFallingEdge();


        lua_State* getLuaState();
        DataVector* getData();

        void test();
    protected:
    private:
        unsigned int id;
        bool stateChanged = false;
        string edge="none";
        string lastState="";
        string hardwareScriptName="";
        lua_State* luaScript;
        DataVector data;


        void templateForOn(std::shared_ptr<luabridge::LuaRef> onFunction);

        std::shared_ptr<luabridge::LuaRef> onHighRef;
        std::shared_ptr<luabridge::LuaRef> onLowRef;
        std::shared_ptr<luabridge::LuaRef> onChangeRef;
        std::shared_ptr<luabridge::LuaRef> onRisingEdgeRef;
        std::shared_ptr<luabridge::LuaRef> onFallingEdgeRef;

};
#endif
