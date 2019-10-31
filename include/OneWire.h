#ifndef ONEWIRE_H
#define ONEWIRE_H
#include <bcm2835.h>
#include <cstdint>
#include<string>
#include<vector>
using namespace std;
#include<math.h>
#define RPI_PIN RPI_GPIO_P1_07
int presence(uint8_t pin);
void writeBitOneWire(uint8_t pin, int b);
void writeByteOneWire(uint8_t pin,int byte);
uint8_t readBitOneWire(uint8_t pin);
int readByteOneWire(uint8_t pin);
void matchROM(uint8_t pin, string device);
void writeByteToDeviceOneWire(uint8_t pin, string device, int data);
int readByteFromDeviceOneWire(uint8_t pin, string device, int reg);
int* readMultipleBytesFromDeviceOneWire(uint8_t pin, string device, int reg, unsigned int amount);

float getDeviceTemperature(uint8_t pin, string device);


int oneWireScan(uint8_t pin, uint64_t serial[]);

uint64_t hexStrToUInt64(string s);
int hexStrToInt(string s);

string intToHexStr(int l);
string intToBinStr(int l);

uint8_t CRC8(vector<uint8_t> data);
#endif

/*
uint8_t CRC8(vector<uint8_t> data)
{
  uint8_t crc = 0;
  for(unsigned int i = 0; i<data.size(); i++)
  {
    uint8_t inbyte = data[i];
    for (uint8_t i = 8; i; i--)
    {
      uint8_t mix = (crc ^ inbyte) & 0x01;
      crc >>= 1;
      if (mix) crc ^= 0x8C;
      inbyte >>= 1;
    }
  }
  return crc;
}
*/
