/*
 * Copyright  Michał Młodawski (SimpleMethod)(c) 2019.
 */
#include <Arduino.h>
#include "SPIFFShandle.hpp"
#include "ArduinoJson.h"

#ifndef _WIFIHANDLE_HPP

#define _WIFIHANDLE_HPP

class wifi_handle
{
private:
  struct ConnectionStatus
  {
    String ssid;
    String ipAddress;
    String hostname;
    int8_t rssi;
    int64_t timer;
  } connectionStatus;

public:
  spiffsfs_handle sp;

  struct Config
  {
    char ssid[64];
    char password[64];
    char refreshtime[10];
  } config;

 explicit wifi_handle(const spiffsfs_handle spValue): sp(spValue)
  {
  }

  void setWifiStatus(const String &ssid, const String &ipAddress, const int8_t rssi, const String &hostname, const int64_t uptime);
  String getSSID();
  String getIpAddress();
  String getHostname();
  int8_t getRSSI();
  int64_t getTimer();
  void loadConfiguration();
  void saveConfiguration(const char *ssid, const char *password, const char *refreshtime);
  void saveConfiguration(const char *ssid, const char *password);
  void saveConfiguration(const char *refreshtime);
  String getConnectionStatus();
};

#endif
