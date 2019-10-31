#include<OneWire.h>

#include<iostream>
#include<iomanip>
#include<sstream>

uint64_t hexStrToUInt64(string s)
{
    uint64_t res =0;
    for(unsigned int i=0;i<s.length();i++)
    {
        uint64_t v =0;
        if(s.at(i) >= '0' && s.at(i) <= '9')
            v = s.at(i) - '0';
        else if(s.at(i) >= 'a' && s.at(i) <= 'f')
            v = s.at(i) - 'a'+10;
        else if(s.at(i) >= 'A' && s.at(i) <= 'F')
            v = s.at(i) - 'A'+10;
        res += v * pow(16,s.length()-i-1);
    }
    return res;
}
int hexStrToInt(string s)
{
    int res =0;
    for(unsigned int i=0;i<s.length();i++)
    {
        int v =0;
        if(s.at(i) >= '0' && s.at(i) <= '9')
            v = s.at(i) - '0';
        else if(s.at(i) >= 'a' && s.at(i) <= 'f')
            v = s.at(i) - 'a'+10;
        else if(s.at(i) >= 'A' && s.at(i) <= 'F')
            v = s.at(i) - 'A'+10;
        res += v * pow(16,s.length()-i-1);
    }
    return res;
}
int presence(uint8_t pin) {
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(pin, LOW);
    bcm2835_delayMicroseconds(480);
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);
    bcm2835_delayMicroseconds(70);
    uint8_t b = bcm2835_gpio_lev(pin);
    bcm2835_delayMicroseconds(410);
    return b;
}
void writeBitOneWire(uint8_t pin, int b) {
    int delay1, delay2;
    if (b == 1) {
        delay1 = 6;
        delay2 = 64;
    } else {
        delay1 = 80;
        delay2 = 10;
    }
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(pin, LOW);
    bcm2835_delayMicroseconds(delay1);
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);
    bcm2835_delayMicroseconds(delay2);
}
void writeByteOneWire(uint8_t pin,int byte) {
    int i;
    for (i = 0; i < 8; i++) {
        if (byte & 1) {
            writeBitOneWire(pin,1);
        } else {
            writeBitOneWire(pin,0);
        }
        byte = byte >> 1;
    }
}
uint8_t readBitOneWire(uint8_t pin) {
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(pin, LOW);
    bcm2835_delayMicroseconds(6);
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);
    bcm2835_delayMicroseconds(8);
    uint8_t b = bcm2835_gpio_lev(pin);
    bcm2835_delayMicroseconds(55);
    return b;
}
int readByteOneWire(uint8_t pin) {
    int byte = 0;
    int i;
    for (i = 0; i < 8; i++) {
        byte = byte | readBitOneWire(pin) << i;
    };
    return byte;
}
void matchROM(uint8_t pin, string device) {
    if(device.length()==16)
    {
        writeByteOneWire(pin, 0x55);
        for (int i = 0; i < 8; i++) {
            string byte = device.substr(device.length()-(2*i)-2,2);
            writeByteOneWire(pin, hexStrToInt(byte));
        }
    }


}
void writeByteToDeviceOneWire(uint8_t pin, string device, int data)
{
    if (presence(pin) == 1) return;

    matchROM(pin, device);
    writeByteOneWire(pin, 0x44);
    presence(pin);
    matchROM(pin, device);
    writeByteOneWire(pin, data);
}
int readByteFromDeviceOneWire(uint8_t pin, string device, int reg)
{
    writeByteToDeviceOneWire(pin,device,reg);
    return readByteOneWire(pin);
}
int* readMultipleBytesFromDeviceOneWire(uint8_t pin, string device, int reg, unsigned int amount)
{
    int* res = new int[amount];
    writeByteToDeviceOneWire(pin,device,reg);
    for(unsigned int i=0; i<amount; i++)
    {
        res[i] = readByteOneWire(pin);
    }

    return res;
}

float getDeviceTemperature(uint8_t pin, string device) {
    if (presence(pin) == 1) return -1000;

    matchROM(pin, device);
    writeByteOneWire(pin,0x44);
    presence(pin);
    matchROM(pin, device);
    writeByteOneWire(pin, 0xBE);

    uint8_t data[9];
    int i;
    for (i = 0; i < 9; i++) {
        data[i] = readByteOneWire(pin);
    }
    //uint8_t crc = crc8(data, 9);
    //if (crc != 0) return -2000;
    int t1 = data[0];
    int t2 = data[1];
    int16_t temp1 = (t2 << 8 | t1);
    float temp = (float) temp1 / 16;
    return temp;
}
/*
float getDeviceTemperature(uint8_t pin, uint64_t device) {
    if (presence(pin) == 1) return -1000;

    matchROM(pin, device);
    writeByteOneWire(pin,0x44);
    presence(pin);
    matchROM(pin, device);
    writeByteOneWire(pin, 0xBE);

    uint8_t data[9];
    int i;
    for (i = 0; i < 9; i++) {
        data[i] = readByteOneWire(pin);
    }
    //uint8_t crc = crc8(data, 9);
    //if (crc != 0) return -2000;
    int t1 = data[0];
    int t2 = data[1];
    int16_t temp1 = (t2 << 8 | t1);
    float temp = (float) temp1 / 16;
    return temp;
}*/

int oneWireScan(uint8_t pin, uint64_t serial[]) {
    static int bitcount = 0;
    static int deviceCount = 0;

    if (bitcount > 63) {
        bitcount = 0;
        deviceCount++;
        return deviceCount;

    }

    if (bitcount == 0) {
        if (presence(pin) == 1) {
            bitcount = 0;
            return deviceCount;
        }
        deviceCount = 0;
        serial[deviceCount] = 0;
        writeByteOneWire(pin, 0xF0);
    };

    int b1 = readBitOneWire(pin);
    int b2 = readBitOneWire(pin);

    if (b1 == 0 && b2 == 1) {
        serial[deviceCount] >>= 1;
        writeBitOneWire(pin, 0);
        bitcount++;
        oneWireScan(pin, serial);
    };

    if (b1 == 1 && b2 == 0) {
        serial[deviceCount] >>= 1;
        serial[deviceCount] |= 0x8000000000000000LL;
        writeBitOneWire(pin, 1);
        bitcount++;
        oneWireScan(pin, serial);

    };

    if (b1 == 1 && b2 == 1) {
        bitcount = 0;
        return deviceCount;
    };

    if (b1 == 0 && b2 == 0) {
        serial[deviceCount] >>= 1;
        writeBitOneWire(pin, 0);
        int bitposition = bitcount;
        bitcount++;
        oneWireScan(pin, serial);

        bitcount = bitposition;
        if (presence(pin) == 1){
            bitposition=0;
            return 0;
        }

        writeByteOneWire(pin, 0xF0);
        uint64_t temp = serial[deviceCount - 1] | (0x1LL << (bitcount));
        int i;
        uint64_t bit;
        for (i = 0; i < bitcount+1; i++) {
            bit = temp & 0x01LL;
            temp >>= 1;
            b1 = readBitOneWire(pin);
            b2 = readBitOneWire(pin);
            writeBitOneWire(pin, bit);
            serial[deviceCount] >>= 1;
            serial[deviceCount] |= (bit << 63);
        }
        bitcount++;
        oneWireScan(pin, serial);
    };

    return deviceCount;
}

string intToHexStr(int l)
{
    std::stringstream stream;
    stream << "0x"
           << std::setfill ('0') << std::setw(2)
           << std::hex << l;
    return stream.str();
}
string intToBinStr(int l)
{
    string res="";

    while (l > 0) {
        int num = l % 2;
        l = l / 2;
        if(num==1)
            res+="1";
        else
            res+="0";
    }
    std::stringstream stream;
    stream << "0x"<< std::setfill ('0') << std::setw(8)<<res;
    return stream.str();
}

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
