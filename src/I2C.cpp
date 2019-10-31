#include<I2C.h>
#include<iostream>

using namespace std;
//------------
void writeByteToRegisterI2C(uint8_t i2cAddr,int regAddress, uint8_t data) {
	char buffer[] = {regAddress, data};

	bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress(i2cAddr);
	bcm2835_i2c_write(buffer, sizeof(buffer));
    bcm2835_i2c_end();
}

void writeBytesToRegisterI2C(uint8_t i2cAddr,int regAddress, vector<uint8_t> data) {
	vector<char> send;
    send.push_back(regAddress);

    for(unsigned int i=0; i<data.size(); i++)
    {
        send.push_back(data[i]);
    }

    char* s = &send[0];

	bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress(i2cAddr);
	bcm2835_i2c_write(s, sizeof(s));
    bcm2835_i2c_end();
}
//--------
int readByteFromRegisterI2C(uint8_t i2cAddr,int regAddress)
{
    char adr[] = {regAddress};
    char* buf = new char[1];
    bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress(i2cAddr);
	bcm2835_i2c_write(adr, 1);
	bcm2835_i2c_read(buf,1);
    bcm2835_i2c_end();
    char res = *buf;
    delete [] buf;
    return res;
}

vector<int> readBytesFromRegisterI2C(uint8_t i2cAddr,int regAddress, unsigned int amount)
{
    char adr[] = {regAddress};
    char* buf = new char[amount];
    bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress(i2cAddr);
	bcm2835_i2c_write(adr, 1);
	bcm2835_i2c_read(buf,amount);
    bcm2835_i2c_end();
    vector<int> res;

    for(unsigned int i=0; i<amount; i++)
    {
        res.push_back(buf[i]);
    }
    delete [] buf;

    return res;
}
