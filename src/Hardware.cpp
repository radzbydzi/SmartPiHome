#include "Hardware.h"
#include<AutoHomePi.h>
#include<fstream>
#include<OneWire.h>
Hardware::Hardware()
{
    initiateScript();
    pin=RPI_GPIO_P1_07;
}
void Hardware::fillAsExistingHardware(unsigned int id)
{
    this->id = id;
}
void Hardware::fillAsNewHardware(string bus, string name, string initialScript, string initialData)
{
    string sql = "INSERT INTO hardware(`bus`,`name`, `initialScript`, `initialData`) VALUES (\""+bus+"\", "+name+", \""+initialScript+"\", \""+initialData+"\")";

    AutoHomePi::sqlExecute(sql);

    vector<vector<string>> hRes = AutoHomePi::sqlGetData("SELECT id FROM hardware ORDER BY id DESC LIMIT 1");

    if(hRes.size()>0)
    {
        if(hRes[0].size()>0)
        {
            id = stoi(hRes[0][0]);
        }
    }
    create();
}
void Hardware::initiateScript()
{
    luaScript = luaL_newstate();
    luaL_openlibs(luaScript);
}

Hardware::~Hardware()
{
    //dtor
}
void Hardware::setId(unsigned int id)
{
    this->id = id;
}
unsigned int Hardware::getId()
{
    return id;
}
void Hardware::setBus(string bus)
{
    string sql = "UPDATE hardware SET `bus`='"+bus+"' WHERE `id`='"+to_string(this->id)+"'";
    AutoHomePi::sqlExecute(sql);
}
string Hardware::getBus()
{
    vector<vector<string>> devRes = AutoHomePi::sqlGetData("SELECT bus FROM hardware WHERE `id`='"+to_string(id)+"' LIMIT 1");

    if(devRes.size()>0)
    {
        if(devRes[0].size()>0)
        {
            return devRes[0][0];
        }
    }
    return "";
}
void Hardware::setAddress(string addr)
{
    string sql = "UPDATE hardware SET `address`='"+addr+"' WHERE `id`='"+to_string(this->id)+"'";
    AutoHomePi::sqlExecute(sql);
}
string Hardware::getAddress()
{

    vector<vector<string>> devRes = AutoHomePi::sqlGetData("SELECT address FROM hardware WHERE `id`='"+to_string(id)+"' LIMIT 1");

    if(devRes.size()>0)
    {
        if(devRes[0].size()>0)
        {
            return devRes[0][0];
        }
    }
    return "";
}

uint8_t Hardware::getAddressAsI2C()
{
    return (uint8_t)getAddressAs1W();
}
uint64_t Hardware::getAddressAs1W()
{
    string s = getAddress();
    uint64_t res =0;
    for(unsigned int i=0;i<s.length();i++)
    {
        uint64_t v =0;
        if(s.at(i) >= '0' && s.at(i) <= '9')
            v = s.at(i) - 48;
        else if(s.at(i) >= 'a' && s.at(i) <= 'f')
            v = s.at(i) - 87;
        else if(s.at(i) >= 'A' && s.at(i) <= 'F')
            v = s.at(i) - 55;

        res += v * pow(16,s.length()-i-1);
    }

    return res;
}
void Hardware::setName(string name)
{
    string sql = "UPDATE hardware SET `name`='"+name+"' WHERE `id`='"+to_string(this->id)+"'";
    AutoHomePi::sqlExecute(sql);
}
string Hardware::getName()
{
    vector<vector<string>> devRes = AutoHomePi::sqlGetData("SELECT name FROM hardware WHERE `id`='"+to_string(id)+"' LIMIT 1");

    if(devRes.size()>0)
    {
        if(devRes[0].size()>0)
        {
            return devRes[0][0];
        }
    }
    return "";
}

void Hardware::setInitialScript(string initialScript)
{
    string sql = "UPDATE hardware SET `initialScript`='"+initialScript+"' WHERE `id`='"+to_string(this->id)+"'";
    AutoHomePi::sqlExecute(sql);
}
string Hardware::getInitialScript()
{
    vector<vector<string>> devRes = AutoHomePi::sqlGetData("SELECT initialScript FROM hardware WHERE `id`='"+to_string(id)+"' LIMIT 1");

    if(devRes.size()>0)
    {
        if(devRes[0].size()>0)
        {
            return devRes[0][0];
        }
    }
    return "";
}

void Hardware::setInitialData(string initialData)
{
    string sql = "UPDATE hardware SET `initialData`='"+initialData+"' WHERE `id`='"+to_string(this->id)+"'";
    AutoHomePi::sqlExecute(sql);
}
string  Hardware::getInitialData()
{
    vector<vector<string>> devRes = AutoHomePi::sqlGetData("SELECT initialData FROM hardware WHERE `id`='"+to_string(id)+"' LIMIT 1");

    if(devRes.size()>0)
    {
        if(devRes[0].size()>0)
        {
            return devRes[0][0];
        }
    }
    return "";
}

void Hardware::addDevice(Device d)
{
    d.setHardwareScriptName(getName());
    devices.push_back(d);
}
Device* Hardware::getDeviceById(unsigned int id)
{
    for(unsigned int i=0; i<devices.size(); i++)
    {
        if(devices[i].getId()==id)
            return &devices[i];
    }
    return nullptr;
}
Device* Hardware::getDeviceByName(string name)
{
    for(unsigned int i=0; i<devices.size(); i++)
    {
        if(devices[i].getName()==name)
            return &devices[i];
    }
    return nullptr;
}
Device* Hardware::getDeviceByNameInScript(string name)
{
    for(unsigned int i=0; i<devices.size(); i++)
    {
        if(devices[i].getNameInScript()==name)
            return &devices[i];
    }
    return nullptr;
}
Device* Hardware::getDeviceByVectorId(unsigned int id)
{
    if(id>=0 && id<devices.size())
        return &devices[id];
    return nullptr;
}
unsigned int Hardware::getDeviceVectorSize()
{
    return devices.size();
}
vector<string> stringSplit(string s, string delimiter)
{
    vector<string> res;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        res.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    res.push_back(s);

    return res;
}
void Hardware::setInitializedData()
{
    string path = getInitialData();

    std::ifstream file(path);

    std::string line;
    while (std::getline(file, line))
    {
        vector<string> l = stringSplit(line, "<:>");
        if(l.size()==2)
        {
            data.setString(l[0],l[1]);
        }
    }
}

DataVector* Hardware::getData()
{
    return &data;
}

void Hardware::create()
{
    if(createRef)
    {
        if((*createRef).isFunction())
        {
            (*createRef)(this);
        }
    }

}

void Hardware::init()
{
    if(initRef)
    {
        if((*initRef).isFunction())
        {
            (*initRef)(this);
        }
    }

}

void Hardware::loop()
{
    if(loopRef)
    {
        if((*loopRef).isFunction())
        {
            (*loopRef)(this);
        }
    }
}
lua_State* Hardware::getLuaState()
{
    return luaScript;
}
bool Hardware::doFile()
{
    string file = getInitialScript();
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
void Hardware::loadScript()
{
     if(doFile())
     {
        initRef = std::make_shared<LuaRef>(getGlobal(luaScript, "init"));
        loopRef = std::make_shared<LuaRef>(getGlobal(luaScript, "loop"));
        setInitializedData();
     }
}

/*
void Hardware::sendByteToBus(int data)
{
    string bus = getBus();
    if(bus=="i2c")
    {
        writeByteToDeviceI2C()
    }
    else if(bus=="1w")
    {
        writeByteOneWire(pin,data);
    }
}
void Hardware::sendByteToRegister(int reg, int data)
{
    string bus = getBus();
    if(bus=="i2c")
    {

    }
    else if(bus=="1w")
    {
        writeByteToDeviceOneWire(pin, 0, data);
    }
}
void Hardware::sendMultipleBytesToRegister(int reg, vector<int> data, unsigned int amount)
{
    string bus = getBus();
    if(bus=="i2c")
    {

    }
    else if(bus=="1w")
    {

    }
}
int Hardware::getByteFromBus()
{
    string bus = getBus();
    if(bus=="i2c")
    {

    }
    else if(bus=="1w")
    {

    }
}
int Hardware::getByteFromRegister(int reg)
{
    string bus = getBus();
    if(bus=="i2c")
    {

    }
    else if(bus=="1w")
    {

    }
}
vector<int> Hardware::getMultipleBytesFromRegister(int reg, unsigned int amount)
{
    string bus = getBus();
    if(bus=="i2c")
    {

    }
    else if(bus=="1w")
    {

    }
}
*/
