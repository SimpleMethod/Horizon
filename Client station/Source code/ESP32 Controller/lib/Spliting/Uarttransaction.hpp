/*
 * Copyright  Michał Młodawski (SimpleMethod)(c) 2019.
 */
#include <regex>
#include <Arduino.h>
#include "ArduinoJson.h"
#include "string.h"

#ifndef _UARTTRANSACTION_HPP
#define _UARTTRANSACTION_HPP
using namespace std;
class data_delivery
{
private:
    struct DeviceData
    {
        String airTemperature;
        String airHumidity;
        String atmosphericPressure;
        String soilTemperature;
        String soilMoisture;
        String FineParticles25;
        String FineParticles100;
        String WindDirection;
        String Radiation;
        int SolarCharging;
        int BatteryVoltage;

    } recoveryData;
    StaticJsonDocument<512> jsonData;
    String jsonParserData;
    vector<string> data;
    /* data */
public:
    string dataParse(String data);
    String getParseJson();
    void dataSearch(string data);

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    int parseJson();

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    String getAirTemperature()
    {
        return this->recoveryData.airTemperature;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    void setAirTemperature(String airTemperature)
    {
        this->recoveryData.airTemperature = airTemperature;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    String getAirHumidity()
    {
        return this->recoveryData.airHumidity;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    void setAirHumidity(String airHumidity)
    {
        this->recoveryData.airHumidity = airHumidity;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    String getAtmosphericPressure()
    {
        return this->recoveryData.atmosphericPressure;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    void setAtmosphericPressure(String atmosphericPressure)
    {
        this->recoveryData.atmosphericPressure = atmosphericPressure;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    String getSoilTemperature()
    {
        return this->recoveryData.soilTemperature;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    void setSoilTemperature(String soilTemperature)
    {
        this->recoveryData.soilTemperature = soilTemperature;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    String getSoilMisture()
    {
        return this->recoveryData.soilMoisture;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    void setSoilMisture(String soilMoisture)
    {
        this->recoveryData.soilMoisture = soilMoisture;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    String getFineParticles25()
    {
        return this->recoveryData.FineParticles25;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    void setFineParticles25(String FineParticles25)
    {
        this->recoveryData.FineParticles25 = FineParticles25;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    String getFineParticles100()
    {
        return this->recoveryData.FineParticles100;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    void setFineParticles100(String FineParticles100)
    {
        this->recoveryData.FineParticles100 = FineParticles100;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    String getWindDirection()
    {
        return this->recoveryData.WindDirection;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    void setWindDirection(String WindDirection)
    {
        this->recoveryData.WindDirection = WindDirection;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    String getRadiation()
    {
        return this->recoveryData.Radiation;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    void setRadiation(String Radiation)
    {
        this->recoveryData.Radiation = Radiation;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    String getJsonData()
    {
        char temp[512];
        serializeJson(this->jsonData, temp);
        return temp;
    }

    [[deprecated("The method is obsolete, use the dataSearch method")]]
    void setJsonData(String jsonData)
    {
        this->jsonData = jsonData;
    }
};

#endif