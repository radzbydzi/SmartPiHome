#include "Device.h"
#include "AutoHomePi.h"


Device::Device()
{
    initiateScript();
}
void Device::fillAsExistingDevice(unsigned int id)
{
    this->id = id;
    //initiateScript();
}
void Device::fillAsNewDevice(string name, unsigned int hardwareId, string state, string description, string type, string script, unsigned int roomId)
{
    //name can be changed, nameInScript cant; nameInScript in one hardware have to be unique
    string sql = "INSERT INTO device(`name`,`nameInScript`,`hardwareId`, `state`, `description`, `type`, `script`, `roomId`) VALUES (\""+name+"\", \""+name+"\", "+to_string(hardwareId)+", \""+state+"\", \""+description+"\", \""+type+"\", \""+script+"\", "+to_string(roomId)+")";
    //cout<<"sql "<<sql<<endl;
    AutoHomePi::sqlExecute(sql);

    vector<vector<string>> devRes = AutoHomePi::sqlGetData("SELECT * FROM device ORDER BY id DESC LIMIT 1");

    if(devRes.size()>0)
    {
        if(devRes[0].size()>0)
        {
            id = stoi(devRes[0][0]);
        }
    }
    //initiateScript();

}

Device::~Device()
{
    //dtor

}
void Device::initiateScript()
{
    luaScript = luaL_newstate();
    luaL_openlibs(luaScript);
}
void Device::setId(unsigned int id)
{
    //string sql = "UPDATE device SET `id`='"+to_string(id)+"' WHERE `id`='"+to_string(this->id)+"'";
    this->id = id;
    //AutoHomePi::sqlExecute(sql);
}
unsigned int Device::getId()
{
    return id;
}

void Device::setName(string name)
{
    string sql = "UPDATE device SET `name`='"+name+"' WHERE `id`='"+to_string(this->id)+"'";
    AutoHomePi::sqlExecute(sql);
}
string Device::getName()
{
    vector<vector<string>> devRes = AutoHomePi::sqlGetData("SELECT name FROM device WHERE `id`='"+to_string(id)+"' LIMIT 1");

    if(devRes.size()>0)
    {
        if(devRes[0].size()>0)
        {
            return devRes[0][0];
        }
    }
    return "";
}

void Device::setNameInScript(string name)
{
    string sql = "UPDATE device SET `nameInScript`='"+name+"' WHERE `id`='"+to_string(this->id)+"'";
    AutoHomePi::sqlExecute(sql);
}
string Device::getNameInScript()
{
    vector<vector<string>> devRes = AutoHomePi::sqlGetData("SELECT nameInScript FROM device WHERE `id`='"+to_string(id)+"' LIMIT 1");

    if(devRes.size()>0)
    {
        if(devRes[0].size()>0)
        {
            return devRes[0][0];
        }
    }
    return "";
}

void Device::setHardwareId(unsigned int hardwareId)
{
    string sql = "UPDATE device SET `hardwareId`='"+to_string(hardwareId)+"' WHERE `id`='"+to_string(this->id)+"'";
    AutoHomePi::sqlExecute(sql);
}
unsigned int Device::getHardwareId()
{
    vector<vector<string>> devRes = AutoHomePi::sqlGetData("SELECT hardwareId FROM device WHERE `id`='"+to_string(id)+"' LIMIT 1");

    if(devRes.size()>0)
    {
        if(devRes[0].size()>0)
        {
            return stoi(devRes[0][0]);
        }
    }
    return 0;
}

void Device::setState(string state)
{
    string sql = "UPDATE device SET `state`='"+state+"' WHERE `id`='"+to_string(this->id)+"'";
    AutoHomePi::sqlExecute(sql);
}
string Device::getState()
{
    vector<vector<string>> devRes = AutoHomePi::sqlGetData("SELECT state FROM device WHERE `id`='"+to_string(id)+"' LIMIT 1");

    if(devRes.size()>0)
    {
        if(devRes[0].size()>0)
        {
            if(lastState!=devRes[0][0])
            {
                setStateChanged(true);
                if(devRes[0][0]=="On" || devRes[0][0]=="High")
                    setEdge("rising");//rising
                else if(devRes[0][0]=="Off" || devRes[0][0]=="Low")
                    setEdge("falling");//falling
                else
                    setEdge("none");

                lastState=devRes[0][0];
            }else
            {
                setStateChanged(false);
            }

            return devRes[0][0];
        }
    }
    return "";
}

void Device::setDescription(string description)
{
    string sql = "UPDATE device SET `description`='"+description+"' WHERE `id`='"+to_string(this->id)+"'";
    AutoHomePi::sqlExecute(sql);
}
string Device::getDescription()
{
    vector<vector<string>> devRes = AutoHomePi::sqlGetData("SELECT description FROM device WHERE `id`='"+to_string(id)+"' LIMIT 1");

    if(devRes.size()>0)
    {
        if(devRes[0].size()>0)
        {
            return devRes[0][0];
        }
    }
    return "";
}

void Device::setType(string type)
{
    string sql = "UPDATE device SET `type`='"+type+"' WHERE `id`='"+to_string(this->id)+"'";
    AutoHomePi::sqlExecute(sql);
}
string Device::getType()
{
    vector<vector<string>> devRes = AutoHomePi::sqlGetData("SELECT type FROM device WHERE `id`='"+to_string(id)+"' LIMIT 1");

    if(devRes.size()>0)
    {
        if(devRes[0].size()>0)
        {
            return devRes[0][0];
        }
    }
    return "";
}

void Device::setScript(string script)
{
    string sql = "UPDATE device SET `script`='"+script+"' WHERE `id`='"+to_string(this->id)+"'";
    AutoHomePi::sqlExecute(sql);
}
string Device::getScript()
{
    vector<vector<string>> devRes = AutoHomePi::sqlGetData("SELECT script FROM device WHERE `id`='"+to_string(id)+"' LIMIT 1");

    if(devRes.size()>0)
    {
        if(devRes[0].size()>0)
        {
            return devRes[0][0];
        }
    }
    return "";
}

void Device::setRoomId(unsigned int roomId)
{
    string sql = "UPDATE device SET `roomId`='"+to_string(roomId)+"' WHERE `id`='"+to_string(this->id)+"'";
    AutoHomePi::sqlExecute(sql);
}
unsigned int Device::getRoomId()
{
    vector<vector<string>> devRes = AutoHomePi::sqlGetData("SELECT roomId FROM device WHERE `id`='"+to_string(id)+"' LIMIT 1");

    if(devRes.size()>0)
    {
        if(devRes[0].size()>0)
        {
            return stoi(devRes[0][0]);
        }
    }
    return -1;
}

void Device::setStateChanged(bool b)
{
    stateChanged=b;
}
bool Device::isStateChanged()
{
    return stateChanged;
}
void Device::setEdge(string s)
{
    edge=s;
}
bool Device::isRisingEdge()
{
    if(edge=="rising")
        return true;
    else
        return false;
}
bool Device::isFallingEdge()
{
    if(edge=="falling")
        return true;
    else
        return false;
}
void Device::setHardwareScriptName(string s)
{
    hardwareScriptName = s;
}
string Device::getHardwareScriptName()
{
    return hardwareScriptName;
}
bool Device::doFile()
{
    string file = getScript();
    if(file!="")
    {
        if (luaL_loadfile(luaScript,  file.c_str()) != LUA_OK) {
            cout<<lua_tostring(luaScript,-1)<<endl;
        }
        lua_pcall(luaScript, 0, 0, 0);
        return true;
    }
    return false;
}
void Device::loadScript()
{
     if(doFile())
     {
        onHighRef = std::make_shared<LuaRef>(getGlobal(luaScript, "onHigh"));
        onLowRef = std::make_shared<LuaRef>(getGlobal(luaScript, "onLow"));
        onChangeRef = std::make_shared<LuaRef>(getGlobal(luaScript, "onChange"));
        onRisingEdgeRef = std::make_shared<LuaRef>(getGlobal(luaScript, "onRisingEdge"));
        onFallingEdgeRef = std::make_shared<LuaRef>(getGlobal(luaScript, "onFallingEdge"));
     }
}

void Device::templateForOn(std::shared_ptr<luabridge::LuaRef> onFunction)
{
    //cout<<onHighRef<<endl;
    if(onFunction)
    {
        if((*onFunction).isFunction())
        {
            try{
                (*onFunction)(this);
            }
            catch(exception & e) {
                cout << e.what() << endl;
            }
        }
    }

}
void Device::onHigh()
{
    templateForOn(onHighRef);
}
void Device::onLow()
{
    templateForOn(onLowRef);
}
void Device::onChange()
{
    templateForOn(onChangeRef);
}
void Device::onRisingEdge()
{
    templateForOn(onRisingEdgeRef);
}
void Device::onFallingEdge()
{
    templateForOn(onFallingEdgeRef);
}
lua_State* Device::getLuaState()
{
    return luaScript;
}

DataVector* Device::getData()
{
    return &data;
}
void Device::test()
{
    cout<<"Works"<<endl;
}

