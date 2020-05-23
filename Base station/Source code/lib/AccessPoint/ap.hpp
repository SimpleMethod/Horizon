/*
 * Copyright  Michał Młodawski (SimpleMethod)(c) 2019.
 */
#ifndef AP_HPP
#define AP_HPP

#include <Arduino.h>
#include <WiFi.h>
#include "esp_wifi.h"
#include <ArduinoJson.h>
const size_t CAPACITY = JSON_ARRAY_SIZE(20);
using namespace std;
class AccessPoint
{
private:

public:
   StaticJsonDocument<CAPACITY> connectedList;
   String prettyConnectedList;
  void getConnectedList();
};

#endif