#include "DataVector.h"
#include<math.h>
DataVector::DataVector()
{
    //ctor
}

DataVector::~DataVector()
{
    //dtor
}
void DataVector::setString(string name, string s)
{
    if(nameExists(name))
    {
        unsigned int id = getIdFromName(name);
        data[id].data=s;
    }else
    {
        data.push_back(dataPortion(name,s));
    }
}
void DataVector::setInt(string name, int i)
{
    try{
        string conv = to_string(i);
        setString(name,conv);
    }
    catch(...)
    {
        cout<<"Cannot convert given int to data object"<<endl;
    }
}
void DataVector::setUInt(string name, unsigned int ui)
{
    try{
        string conv = to_string(ui);
        setString(name,conv);
    }
    catch(...)
    {
        cout<<"Cannot convert given uint to data object"<<endl;
    }
}
void DataVector::setDouble(string name, double d)
{
    try{
        string conv = to_string(d);
        setString(name,conv);
    }
    catch(...)
    {
        cout<<"Cannot convert given double to data object"<<endl;
    }
}
void DataVector::setFloat(string name, float f)
{
    try{
        string conv = to_string(f);
        setString(name,conv);
    }
    catch(...)
    {
        cout<<"Cannot convert given float to data object"<<endl;
    }
}

string DataVector::getAsString(string name)
{
    for(unsigned int i = 0; i<data.size(); i++)
    {
        if(name == data[i].name)
            return data[i].data;
    }
    return "";
}
int DataVector::getAsInt(string name)
{
    try{
        return stoi(getAsString(name));
    }
    catch(...)
    {
        cout<<"Cannot convert given data object to int"<<endl;
        return NULL;
    }
}
unsigned int DataVector::getAsUInt(string name)
{
    try{
        return stoul(getAsString(name));
    }
    catch(...)
    {
        cout<<"Cannot convert given data object to uint"<<endl;
        return NULL;
    }
}
double DataVector::getAsDouble(string name)
{
    try{
        return stod(getAsString(name));
    }
    catch(...)
    {
        cout<<"Cannot convert given data object to double"<<endl;
        return NULL;
    }
}
float DataVector::getAsFloat(string name)
{
    try{
        return stof(getAsString(name));
    }
    catch(...)
    {
        cout<<"Cannot convert given data object to float"<<endl;
        return NULL;
    }
}

unsigned char DataVector::getHexAsUChar(string name)
{
    return (unsigned char)getHexAsUInt64(name);
}
uint64_t DataVector::getHexAsUInt64(string name)
{
    string s = getAsString(name);
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

unsigned char DataVector::getBinAsUChar(string name)
{
    return (unsigned char)getBinAsUInt64(name);
}
uint64_t DataVector::getBinAsUInt64(string name)
{
    string s = getAsString(name);
    uint64_t res =0;
    for(unsigned int i=0;i<s.length();i++)
    {
        uint8_t v =0;
        if(s.at(i) == '1')
            v = 1;

        res += v * pow(2,s.length()-i-1);
    }
    return res;
}

string DataVector::getElementById(unsigned int id)
{
    if(id<0 || id>data.size()-1)
        return "";
    else
        return data[id].data;
}
unsigned int DataVector::size()
{
    return data.size();
}
bool DataVector::nameExists(string name)
{
    for(unsigned int i = 0; i<data.size(); i++)
    {
        if(name == data[i].name)
            return true;
    }
    return false;
}
unsigned int DataVector::getIdFromName(string name)
{
    for(unsigned int i = 0; i<data.size(); i++)
    {
        if(name == data[i].name)
            return i;
    }
    return NULL;
}
