#include "HardwareList.h"

HardwareList::HardwareList()
{
    //ctor
}

HardwareList::~HardwareList()
{
    //dtor
}

void HardwareList::addHardware(Hardware h)
{
    hardwares.push_back(h);
}
Hardware* HardwareList::getHardware(unsigned int id)
{
    for(unsigned int i=0; i<hardwares.size(); i++)
    {
        if(hardwares[i].getId()==id)
            return &hardwares[i];
    }
    return nullptr;
}
Hardware* HardwareList::getHardwareByName(string name)
{
    for(unsigned int i=0; i<hardwares.size(); i++)
    {
        if(hardwares[i].getName()==name)
            return &hardwares[i];
    }
    return nullptr;
}
Hardware* HardwareList::getHardwareByVectorId(unsigned int id)
{
    if(id>=0 && id<hardwares.size())
        return &hardwares[id];
    else
        return nullptr;
}
unsigned int HardwareList::getHardwareAmount()
{
    return hardwares.size();
}
