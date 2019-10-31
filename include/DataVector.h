#ifndef DATAVECTOR_H
#define DATAVECTOR_H

#include<vector>
#include<iostream>
#include<string>

using namespace std;


class DataVector
{
    public:
        struct dataPortion
        {
            public:
            dataPortion(string n, string d)
            {
                name=n;
                data=d;
            }
            string name;
            string data;
        };
        DataVector();
        virtual ~DataVector();

        //Dont forget to implement all of methods into lua
        void setString(string name, string s);
        void setInt(string name, int i);
        void setUInt(string name, unsigned int ui);
        void setDouble(string name, double d);
        void setFloat(string name, float f);

        string getAsString(string name);
        int getAsInt(string name);
        unsigned int getAsUInt(string name);
        double getAsDouble(string name);
        float getAsFloat(string name);

        unsigned char getHexAsUChar(string name);
        uint64_t getHexAsUInt64(string name);

        unsigned char getBinAsUChar(string name);
        uint64_t getBinAsUInt64(string name);

        string getElementById(unsigned int id);
        unsigned int size();
        bool nameExists(string name);
        unsigned int getIdFromName(string name);

    protected:
    private:
        vector<dataPortion> data;
};

#endif // DATAVECTOR_H
