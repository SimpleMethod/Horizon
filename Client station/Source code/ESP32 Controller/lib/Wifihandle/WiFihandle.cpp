/*
 * Copyright  Michał Młodawski (SimpleMethod)(c) 2019.
 */
#include "WiFihandle.hpp"

void wifi_handle::loadConfiguration()
{
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, sp.readFile("/wifi.bin"));
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  strlcpy(config.ssid, doc["ssid"], sizeof(config.ssid));
  strlcpy(config.password, doc["password"], sizeof(config.password));
  strlcpy(config.refreshtime, doc["refreshtime"], sizeof(config.refreshtime));
};

void wifi_handle::saveConfiguration(const char *ssid, const char *password, const char *refreshtime)
{
  char temp[200];
  StaticJsonDocument<200> doc;
  doc["ssid"] = ssid;
  doc["password"] = password;
  doc["refreshtime"] = refreshtime;

  serializeJson(doc, temp);
  sp.writeFile("/wifi.bin", temp);
};

void wifi_handle::saveConfiguration(const char *ssid, const char *password)
{
  wifi_handle::loadConfiguration();
  char temp[200];
  StaticJsonDocument<200> doc;
  doc["ssid"] = ssid;
  doc["password"] = password;
  doc["refreshtime"] = config.refreshtime;

  serializeJson(doc, temp);
  sp.writeFile("/wifi.bin", temp);
}

void wifi_handle::saveConfiguration(const char *refreshtime)
{
  wifi_handle::loadConfiguration();
  char temp[200];
  StaticJsonDocument<200> doc;
  doc["ssid"] = config.ssid;
  doc["password"] = config.password;
  doc["refreshtime"] = refreshtime;

  serializeJson(doc, temp);
  sp.writeFile("/wifi.bin", temp);
}

void wifi_handle::setWifiStatus(const String &SSID, const String &ipAddress, const int8_t RSSI, const String &hostname, const int64_t timer)
{
  this->connectionStatus.ssid = SSID;
  this->connectionStatus.ipAddress = ipAddress;
  this->connectionStatus.rssi = RSSI;
  this->connectionStatus.hostname = hostname;
  this->connectionStatus.timer = timer;
};

String wifi_handle::getSSID()
{
  return this->connectionStatus.ssid;
}
String wifi_handle::getIpAddress()
{
  return this->connectionStatus.ipAddress;
}
String wifi_handle::getHostname()
{
  return this->connectionStatus.hostname;
}
int8_t wifi_handle::getRSSI()
{
  return this->connectionStatus.rssi;
}
int64_t wifi_handle::getTimer()
{
  return this->connectionStatus.timer;
}

String wifi_handle::getConnectionStatus()
{
  String temp;
  StaticJsonDocument<200> doc;
  doc["ssid"] = this->connectionStatus.ssid;
  doc["ip_address"] = this->connectionStatus.ipAddress;
  doc["rssi"] = this->connectionStatus.rssi;
  doc["hostname"] = this->connectionStatus.hostname;
  doc["timer"] = this->connectionStatus.timer;

  serializeJson(doc, temp);
  return temp;
}