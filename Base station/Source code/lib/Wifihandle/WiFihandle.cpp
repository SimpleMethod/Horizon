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
};

void wifi_handle::saveConfiguration(const char *ssid, const char *password)
{
  char temp[200];
  StaticJsonDocument<200> doc;
  doc["ssid"] = ssid;
  doc["password"] = password;

  serializeJson(doc, temp);
  sp.writeFile("/wifi.bin", temp);
};