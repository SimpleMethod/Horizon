/*
 * Copyright  Michał Młodawski (SimpleMethod)(c) 2019.
 */
#include "Uarttransaction.hpp"

using namespace std;

#define DATA_DELIVERY_DEBUG false

void data_delivery::dataSearch(string data)
{
    this->data.clear();
    char delim[] = ";";
    char *token = strtok((char *)data.c_str(), delim);
    while (token)
    {
        this->data.push_back(token);
        token = strtok(NULL, delim);
    }

    if (this->data.size() == 9)
    {
        this->recoveryData.airTemperature = this->data.at(0).c_str();
        this->recoveryData.airHumidity = this->data.at(1).c_str();
        this->recoveryData.atmosphericPressure = this->data.at(2).c_str();
        this->recoveryData.soilTemperature = this->data.at(3).c_str();
        this->recoveryData.soilMoisture = this->data.at(4).c_str();
        this->recoveryData.FineParticles25 = this->data.at(5).c_str();
        this->recoveryData.FineParticles100 = this->data.at(6).c_str();
        this->recoveryData.WindDirection = this->data.at(7).c_str();
        this->recoveryData.Radiation = this->data.at(8).c_str();

#if DATA_DELIVERY_DEBUG
        Serial.println(this->data.at(0).c_str());
        Serial.println(this->data.at(1).c_str());
        Serial.println(this->data.at(2).c_str());
        Serial.println(this->data.at(3).c_str());
        Serial.println(this->data.at(4).c_str());
        Serial.println(this->data.at(5).c_str());
        Serial.println(this->data.at(6).c_str());
        Serial.println(this->data.at(7).c_str());
        Serial.println(this->data.at(8).c_str());
#endif

        this->jsonParserData = "";
        this->jsonData["horizon_id"] = "153b949912fb4795";
        this->jsonData["air_temperature"] = atof(this->data.at(0).c_str());
        this->jsonData["air_humidity"] = atof(this->data.at(1).c_str());
        this->jsonData["atmospheric_pressure"] = atof(this->data.at(2).c_str());
        this->jsonData["soil_temperature"] = atof(this->data.at(3).c_str());
        this->jsonData["soil_moisture"] = atof(this->data.at(4).c_str());
        this->jsonData["fine_particles25"] = atof(this->data.at(5).c_str());
        this->jsonData["fine_particles100"] =  atof(this->data.at(6).c_str());
        this->jsonData["wind_direction"] = atof(this->data.at(7).c_str());
        this->jsonData["average_radiation"] =  atof(this->data.at(8).c_str());
        this->jsonData["battery_voltage"]=this->recoveryData.BatteryVoltage;
        this->jsonData["solar_charging"]=this->recoveryData.SolarCharging;

        serializeJson(this->jsonData, this->jsonParserData);
#if DATA_DELIVERY_DEBUG
        Serial.println(getParseJson());
#endif

    }
}
#if DATA_DELIVERY_DEBUG
int data_delivery::parseJson()
{
    this->jsonParserData = "";
    this->jsonData["air_temperature"] = getAirTemperature();
    this->jsonData["air_humidity"] = getAirHumidity();
    this->jsonData["atmospheric_pressure"] = getAtmosphericPressure();
    this->jsonData["soil_temperature"] = getSoilTemperature();
    this->jsonData["soil_misture"] = getSoilMisture();
    this->jsonData["fine_particles25"] = getFineParticles25();
    this->jsonData["fine_particles100"] = getFineParticles100();
    this->jsonData["wind_direction"] = getWindDirection();
    this->jsonData["average_radiation"] = getRadiation();
    this->jsonData["horizon_id"] = "153b949912fb4795";

    try
    {
        if (serializeJson(this->jsonData, this->jsonParserData) == 0)
        {
            throw - 1;
        }
        else
        {
            return 1;
        }
    }
    catch (int e)
    {
        return e;
    }
}
#endif
String data_delivery::getParseJson()
{
    return this->jsonParserData;
}
std::string data_delivery::dataParse(String data)
{
    char tempArray[200];
    data.toCharArray(tempArray, 200);
    std::string s(tempArray);
    return s;
}