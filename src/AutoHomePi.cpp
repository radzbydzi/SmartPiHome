#include "AutoHomePi.h"
#include <sqlite3.h>
#include<LuaBridge/Vector.h>

sqlite3 *AutoHomePi::db=0;
AutoHomePi::AutoHomePi(const AutoHomePi& a)
{

    cout<<"Pierwsza opcja"<<endl;
}
AutoHomePi::AutoHomePi()
{
    //gpio i2c 1w

    hlist = new HardwareList();
    //db connection
    int rc = sqlite3_open("/home/pi/database.db", &AutoHomePi::db);

    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(AutoHomePi::db));
    } else {
        fprintf(stdout, "Opened database successfully\n");
    }
    //add every existing hardware and devices
    vector<vector<string>> hard = sqlGetData("SELECT id FROM hardware");

    for(unsigned int i=0; i<hard.size(); i++)
    {
        vector<vector<string>> dev = sqlGetData("SELECT id FROM device WHERE hardwareId='"+hard[i][0]+"'");
        Hardware h = Hardware();
        h.fillAsExistingHardware(stoi(hard[i][0]));
        for(unsigned int j=0; j<dev.size(); j++)
        {
            cout<<"j "<<j<<endl;
            Device d = Device();
            d.fillAsExistingDevice(stoi(dev[j][0]));
            h.addDevice(d);
        }
        hlist->addHardware(h);
    }

}

AutoHomePi::~AutoHomePi()
{
    //sqlite3_close(AutoHomePi::db);
    //cout<<"Db Closed"<<endl;
}

void AutoHomePi::startThreads()
{
    //thread initialization
    luaThread = std::thread(&AutoHomePi::luaInterpreter, this);
    httpThread = std::thread(&AutoHomePi::httpServer,this);
    websocketThread = std::thread(&AutoHomePi::webSocketServer, this);
}
void AutoHomePi::waitForThreadsStop()
{
    cout<<"lua"<<endl;
    if(luaThread.joinable())
        luaThread.join();
    cout<<"http"<<endl;
    if(httpThread.joinable())
        httpThread.join();
    cout<<"ws"<<endl;
    if(websocketThread.joinable())
        websocketThread.join();
}
void AutoHomePi::test()
{
    cout<<"Works"<<endl;
}
void addBuses(lua_State* l)
{
    getGlobalNamespace(l)
        .beginNamespace("Bus")
            .beginNamespace("I2C")
                .addFunction ("writeByteToRegister", &writeByteToRegisterI2C)
                .addFunction ("writeBytesToRegister", &writeBytesToRegisterI2C)
                .addFunction ("readByteFromRegister", &readByteFromRegisterI2C)
                .addFunction ("readBytesFromRegister", &readBytesFromRegisterI2C)
                .addFunction ("hexStrToInt", &hexStrToInt)
                .addFunction ("intToHexStr", &intToHexStr)
                .addFunction ("intToBinStr", &intToBinStr)

                .addFunction ("setBaudrate", &bcm2835_i2c_set_baudrate)
                .addFunction ("setClockDivider", &bcm2835_i2c_setClockDivider)
            .endNamespace()
            .beginNamespace("OneWire")
                .addFunction ("presence", &presence)
                .addFunction ("writeBit", &writeBitOneWire)
                .addFunction ("writeByte", &writeByteOneWire)
                .addFunction ("readBit", &readBitOneWire)
                .addFunction ("readByte", &readByteOneWire)
                .addFunction ("matchROM", &matchROM)
                .addFunction ("writeByteToDevice", &writeByteToDeviceOneWire)
                .addFunction ("readMultipleBytesFromDevice", &readMultipleBytesFromDeviceOneWire)
                .addFunction ("getDeviceTemperature", &getDeviceTemperature)
                .addFunction ("hexStrToInt", &hexStrToInt)
                .addFunction ("intToBinStr", &intToBinStr)
                .addFunction ("CRC8", &CRC8)

            .endNamespace()
        .endNamespace();
}
void addVectorClass(lua_State* l)
{
    getGlobalNamespace(l)
        .beginNamespace("Core")
            .beginClass <DataVector> ("Vector")
                .addFunction ("setString", &DataVector::setString)
                .addFunction ("setInt", &DataVector::setInt)
                .addFunction ("setUInt", &DataVector::setUInt)
                .addFunction ("setDouble", &DataVector::setDouble)
                .addFunction ("setFloat", &DataVector::setFloat)
                .addFunction ("getAsString", &DataVector::getAsString)
                .addFunction ("getAsInt", &DataVector::getAsInt)
                .addFunction ("getAsUInt", &DataVector::getAsUInt)
                .addFunction ("getAsDouble", &DataVector::getAsDouble)
                .addFunction ("getAsFloat", &DataVector::getAsFloat)
                .addFunction ("getHexAsUChar", &DataVector::getHexAsUChar)
                .addFunction ("getHexAsUInt64", &DataVector::getHexAsUInt64)
                .addFunction ("getBinAsUChar", &DataVector::getBinAsUChar)
                .addFunction ("getBinAsUInt64", &DataVector::getBinAsUInt64)
                .addFunction ("getElementById", &DataVector::getElementById)
                .addFunction ("size", &DataVector::size)
                .addFunction ("nameExists", &DataVector::nameExists)
            .endClass ()
        .endNamespace();
}
void addDeviceClass(lua_State* l)
{
    getGlobalNamespace(l)
        .beginNamespace("Core")
            .beginClass <Device> ("Device")
                    .addConstructor<void (*)(void)>()
                    .addFunction ("fillAsExistingDevice", &Device::fillAsExistingDevice)
                    .addFunction ("fillAsNewDevice", &Device::fillAsNewDevice)
                    .addFunction ("setId", &Device::setId)
                    .addFunction ("getId", &Device::getId)
                    .addFunction ("setName", &Device::setName)
                    .addFunction ("getName", &Device::getName)
                    //.addFunction ("setNameInScript", &Device::setNameInScript)
                    .addFunction ("getNameInScript", &Device::getNameInScript)
                    .addFunction ("setHardwareId", &Device::setHardwareId)
                    .addFunction ("getHardwareId", &Device::getHardwareId)
                    .addFunction ("setState", &Device::setState)
                    .addFunction ("getState", &Device::getState)
                    .addFunction ("setDescription", &Device::setDescription)
                    .addFunction ("getDescription", &Device::getDescription)
                    .addFunction ("setType", &Device::setType)
                    .addFunction ("getType", &Device::getType)
                    .addFunction ("setScript", &Device::setScript)
                    .addFunction ("getScript", &Device::getScript)
                    .addFunction ("setRoomId", &Device::setRoomId)
                    .addFunction ("getRoomId", &Device::getRoomId)
                    .addFunction("getData", &Device::getData)

                    .addFunction ("setHardwareScriptName", &Device::setHardwareScriptName)
                    .addFunction("getHardwareScriptName", &Device::getHardwareScriptName)
            .endClass ()
        .endNamespace();
}
void addHardwareClass(lua_State* l)
{
    getGlobalNamespace(l)
        .beginNamespace("Core")
            .beginClass <Hardware> ("Hardware")
                    .addFunction ("fillAsExistingHardware", &Hardware::fillAsExistingHardware)
                    .addFunction("setId", &Hardware::setId)
                    .addFunction("getId", &Hardware::getId)
                    .addFunction("setBus", &Hardware::setBus)
                    .addFunction("getBus", &Hardware::getBus)
                    .addFunction("setAddress", &Hardware::setAddress)
                    .addFunction("getAddress", &Hardware::getAddress)
                    .addFunction("setName", &Hardware::setName)
                    .addFunction("getName", &Hardware::getName)
                    .addFunction("getInitialData", &Hardware::getInitialData)
                    .addFunction("getData", &Hardware::getData)
                    .addFunction("addDevice", &Hardware::addDevice)
                    .addFunction("getDeviceById", &Hardware::getDeviceById)
                    .addFunction("getDeviceByName", &Hardware::getDeviceByName)
                    .addFunction("getDeviceByNameInScript", &Hardware::getDeviceByNameInScript)
                    .addFunction("getDeviceByVectorId", &Hardware::getDeviceByVectorId)
                    .addFunction("getDeviceVectorSize", &Hardware::getDeviceVectorSize)

                    .addFunction("getAddressAsI2C", &Hardware::getAddressAsI2C)
                    .addFunction("getAddressAs1W", &Hardware::getAddressAs1W)
                    //.addFunction("sendByteToBus", &Hardware::sendByteToBus)
                    //.addFunction("sendByteToRegister", &Hardware::sendByteToRegister)
                    //.addFunction("sendMultipleBytesToRegister", &Hardware::sendMultipleBytesToRegister)
                    //.addFunction("getByteFromBus", &Hardware::getByteFromBus)
                    //.addFunction("getByteFromRegister", &Hardware::getByteFromRegister)
                    //.addFunction("getMultipleBytesFromRegister", &Hardware::fillAsNewHardware)
            .endClass ()
        .endNamespace();
}
void addAutoHomePiClass(lua_State* l)
{
    getGlobalNamespace(l)
        .beginNamespace("Core")
            .beginClass <AutoHomePi> ("AutoHomePi")
                .addConstructor <void (*) (void)> ()
                .addFunction ("test", &AutoHomePi::test)
                .addStaticFunction ("sqlExecute", &AutoHomePi::sqlExecute)
            .endClass ()
        .endNamespace();

    getGlobalNamespace(l)
        .beginNamespace("Core")
            .beginClass <HardwareList> ("HardwareList")
                .addFunction ("getHardware", &HardwareList::getHardware)
                .addFunction ("getHardwareByName", &HardwareList::getHardwareByName)
                .addFunction ("getHardwareAmount", &HardwareList::getHardwareAmount)
            .endClass ()
        .endNamespace();
}
//--

void sleep(unsigned int micro)
{
    std::this_thread::sleep_for(std::chrono::microseconds(micro));
}
void AutoHomePi::luaInterpreter()
{

    for(unsigned int j=0; j<hlist->getHardwareAmount(); j++)
    {
        Hardware* h = hlist->getHardwareByVectorId(j);
        //add classes to lua
        addBuses(h->getLuaState());
        addVectorClass(h->getLuaState());
        addDeviceClass(h->getLuaState());
        addAutoHomePiClass(h->getLuaState());
        addHardwareClass(h->getLuaState());

        //add Objects to lua
        getGlobalNamespace(h->getLuaState())
        .beginNamespace("Core")
            .addVariable("System", &hlist)
            .addFunction("sleep", &sleep)
        .endNamespace();

        //load lua file
        h->loadScript();


        for(int i=0; i<hlist->getHardwareByVectorId(j)->getDeviceVectorSize();i++)
        {
            Device* a = hlist->getHardwareByVectorId(j)->getDeviceByVectorId(i);

            //add classes to device lua
            addVectorClass(a->getLuaState());
            addDeviceClass(a->getLuaState());
            addAutoHomePiClass(a->getLuaState());
            addHardwareClass(a->getLuaState());
            //add objects to device lua
            getGlobalNamespace(a->getLuaState())
            .beginNamespace("Core")
                .addVariable  ("System", &hlist)
                .addFunction("sleep", &sleep)
            .endNamespace();
            //load device lua script
            a->loadScript();

        }
        //init function of harware
        h->init();
    }

    while(running)
    {
        for(unsigned int j=0; j<hlist->getHardwareAmount(); j++)
        {
            Hardware* h = hlist->getHardwareByVectorId(j);
            //h->doFile();
            h->loop();
            for(int i=0; i<h->getDeviceVectorSize();i++)
            {

                //cout<<"i "<<i<<endl;
                Device* a = h->getDeviceByVectorId(i);
                //a->doFile();
                string state = a->getState();
                if(state=="On")
                    a->onHigh();
                else if(state=="Off")
                    a->onLow();

                if(a->isStateChanged())
                {
                    a->onChange();

                    if(a->isRisingEdge())
                        a->onRisingEdge();
                    else if(a->isFallingEdge())
                        a->onFallingEdge();
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
void AutoHomePi::httpServer()
{
    while(running)
    {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}
void AutoHomePi::webSocketServer()
{
    while(running)
    {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}
//--
vector<vector<string>> AutoHomePi::sqlGetData(string sql)
{
    vector<vector<string>> res;

    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(AutoHomePi::db, sql.c_str(), -1, &stmt, 0)== SQLITE_OK)
    {
        if(sqlite3_step(stmt) == SQLITE_ROW)
        {
            while( sqlite3_column_text( stmt, 0 ) )
            {
                vector<string> row;
                for( int i = 0; i < sqlite3_data_count(stmt); i++ )
                    row.push_back(std::string( (char *)sqlite3_column_text( stmt, i ) ));
                res.push_back(row);
                sqlite3_step( stmt );
            }
        }else
        {
            cout<<"("<<sql<<")"<<"Query gives no records"<<endl;
        }
    }else
    {
        cout<<"Error: "<<sqlite3_errmsg(AutoHomePi::db)<<endl;
    }
    sqlite3_finalize(stmt);
    return res;
}
void AutoHomePi::sqlExecute(string sql)
{
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(AutoHomePi::db, sql.c_str(), -1, &stmt, 0);
    //sqlite3_step(stmt);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        cout<<sql<<endl;
        cout<<"Error in execute query"<<endl;;
    }
    sqlite3_finalize(stmt);
}
