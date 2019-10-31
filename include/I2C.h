#ifndef I2C_H
#define I2C_H
#include <bcm2835.h>
#include <cstdint>
#include <vector>
#include<I2C.h>

using namespace std;
//------------
void writeByteToRegisterI2C(uint8_t i2cAddr,int regAddress, uint8_t data);
void writeBytesToRegisterI2C(uint8_t i2cAddr,int regAddress, vector<uint8_t> data);
//--------
int readByteFromRegisterI2C(uint8_t i2cAddr,int regAddress);
vector<int> readBytesFromRegisterI2C(uint8_t i2cAddr,int regAddress, unsigned int amount);


#endif

