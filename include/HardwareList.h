#ifndef HARDWARELIST_H
#define HARDWARELIST_H

#include<Hardware.h>

class HardwareList
{
    public:
        HardwareList();
        virtual ~HardwareList();

        void addHardware(Hardware h);
        Hardware* getHardware(unsigned int id);
        Hardware* getHardwareByName(string name);
        Hardware* getHardwareByVectorId(unsigned int id);

        unsigned int  getHardwareAmount();


    protected:
    private:
        vector<Hardware> hardwares;//all hardwares
};

#endif // HARDWARELIST_H
