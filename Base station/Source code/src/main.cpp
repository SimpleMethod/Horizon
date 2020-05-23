/*
 * Copyright  Michał Młodawski (SimpleMethod)(c) 2019.
 */
#include <esp_wifi.h>
#include "ESPAsyncWebServer.h"
#include <string>
#include <iostream>
#include <regex>
#include "ArduinoJson.h"
#include <FS.h>
#include "SPIFFS.h"
#include "sim.hpp"
#include "ap.hpp"
#include "SPIFFShandle.hpp"
#include "WiFihandle.hpp"
#include <ESPmDNS.h>

#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"


using namespace std;

#define FORMAT_SPIFFS_IF_FAILED true
#define NORMAL_MODE true
#define ENABLE_MDNS true

#if NORMAL_MODE
IPAddress ap_local_IP(192, 168, 1, 1);
IPAddress ap_gateway(192, 168, 1, 254);
IPAddress ap_subnet(255, 255, 255, 0);
const static wifi_country_t wifi_country = {.cc = "PL",
                                            .schan = 1,
                                            .nchan = 13,
                                            .max_tx_power = 126,
                                            .policy = WIFI_COUNTRY_POLICY_AUTO};
#endif

AsyncWebServer server(80);
spiffsfs_handle sph;
wifi_handle wifihandle(sph);
AccessPoint ap;
Sim sim;
unsigned long startTime = 0;
unsigned long previousTimeUpdate = 0;
unsigned long previousTimeStationList = 0;
uint8_t semaphore = 0;

boolean connectToNetwork(const char *hostname);

void setup() {
//disableCore0WDT();
//disableCore1WDT();
//disableLoopWDT();
pinMode(32, OUTPUT);
pinMode(33, OUTPUT);

  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  if (!sph.checkFile("/wifi.bin")) {
    sph.writeFile("/wifi.bin",
                  "{ \"ssid\":\"horzion\", \"password\": \"horizon2137\"}");
  }

  wifihandle.loadConfiguration();
  digitalWrite(32,1);

  delay(1000);
digitalWrite(32,0);
  connectToNetwork("gatewayhorizon");

  Serial2.begin(115200, SERIAL_8N1, 19, 18);
  Serial.begin(115200);
  Serial.println(F("Configuring access point..."));

  server.on("/upload/data", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
            [](AsyncWebServerRequest *request, uint8_t *data, size_t len,
               size_t index, size_t total) mutable {
              digitalWrite(33, HIGH);
              string _BigData = string(data, data + len);
              sim.sim7000eLaunch();
              Serial.printf("[REQUEST]\t%s\r\n", _BigData.c_str());
              sim.sendpost("http://d2fw69tfosfc4v.cloudfront.net/measurement",
                           _BigData.c_str());
              _BigData = "";
              digitalWrite(33, LOW);
              request->send(200);
            });

  server.on("/connection", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/wifi.bin");
  });


 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(SPIFFS, "/index.html");});
 server.on("/connection", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(SPIFFS, "/wifi.bin");});
 server.on("/fonts/icomoon.eot", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(SPIFFS, "/fonts/icomoon.eot");});
 server.on("/fonts/icomoon.svg", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(SPIFFS, "/fonts/icomoon.svg");});
 server.on("/fonts/icomoon.ttf", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(SPIFFS, "/fonts/icomoon.ttf");});
 server.on("/fonts/icomoon.woff", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(SPIFFS, "/fonts/icomoon.woff");});
 server.on("/angular.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(SPIFFS, "/angular.min.js");});
 server.on("/angular-route.js", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(SPIFFS, "/angular-route.js");});
 server.on("/app.js", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(SPIFFS, "/app.js");});
 server.on("/aws.png", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(SPIFFS, "/aws.png");});
 server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(SPIFFS, "/bootstrap.min.css");});
 server.on("/home.html", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(SPIFFS, "/home.html");});
 server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(SPIFFS, "/index.html");});
 server.on("/jquery.easing.js", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(SPIFFS, "/jquery.easing.js");});
 server.on("/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(SPIFFS, "/jquery.min.js");});
 server.on("/settings.html", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(SPIFFS, "/settings.html");});
 server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(SPIFFS, "/style.css");});
 server.on("/theme.js", HTTP_GET, [](AsyncWebServerRequest *request) {request->send(SPIFFS, "/theme.js");});




  server.on("/connection", HTTP_POST, [](AsyncWebServerRequest *request) {
    const char *ssid = 0;
    const char *password = 0;
    int temp = 0;
    if (request->hasParam("ssid")) {
      AsyncWebParameter *p = request->getParam("ssid");
      ssid = p->value().c_str();
      temp++;
      if (p->value().isEmpty()) {
        request->send(404, "text/html", "error");
      }
    }
    if (request->hasParam("password")) {
      AsyncWebParameter *p = request->getParam("password");
      password = p->value().c_str();
      temp++;
      if (p->value().isEmpty()) {
        request->send(404, "text/html", "error");
      }
    }
    if (temp >= 1) {
      wifihandle.saveConfiguration(ssid, password);
      request->send(200, "text/html", "ok");
    } else {
      request->send(500, "text/html", "error");
    }
  });

  server.on("/gsm/restart", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (sim.sim7000eLaunch() == true) {
      request->send(200, "text/html", "ok");
    } else {
      request->send(500, "text/html", "error");
    }
  });

  server.on("/devices/list", HTTP_GET, [](AsyncWebServerRequest *request) {
#if NORMAL_MODE
    ap.getConnectedList();
    request->send(200, "text/html", ap.prettyConnectedList);
#else
     string str = R"(["192.168.1.1","127.0.0.0"]])";
    request->send(200, "text/html", ap.prettyConnectedList);
#endif

  });

  server.on("/gsm/info", HTTP_GET, [](AsyncWebServerRequest *request) {
    StaticJsonDocument<512> doc;
    string _temp;
    String temp;
    char json_string[512];

    _temp = sim.getLocalNetwork();
    temp = _temp.c_str();
    if (_temp == "Not found") {
      doc["local_network"] = F("0");
    } else {
      doc["local_network"] = temp;
    }
    _temp = sim.getSignalStrenght();
    temp = _temp.c_str();
    if (_temp == "Not found") {
      doc["signal_strenght"] = F("99");
    } else {
      doc["signal_strenght"] = temp;
    }
    _temp = sim.getIPAddress();
    temp = _temp.c_str();
    if (_temp == "Not found") {
      doc["IPAddress"] = F("0");
    } else {
      doc["IPAddress"] = temp;
    }

    _temp = sim.getAttachedToNetwork();
    temp = _temp.c_str();
    if (_temp == "Not found") {
      doc["attached_to_network"] = F("0");
    } else {
      doc["attached_to_network"] = temp;
    }

    serializeJson(doc, json_string);
    request->send(200, "application/json", json_string);
    sim.sim7000Stack.clear();
  });
  server.begin();
  Serial.println(F("Server started"));
}

void loop() {
  startTime = millis();
  if (startTime - previousTimeUpdate > 60000) {
    if (semaphore == 0) {
      sim.sim7000eLaunch();
      delay(10);
      if (sim.getLocalNetwork() == "Not found") {
        Serial.println(F("Błąd inicjalizacji polaczenia"));
      } else {
        sim.sendpost("http://httpbin.org/post", "null");
        Serial.println(F("Uruchomiono modul sim7000e"));
      }
      semaphore = 1;
    }
    previousTimeUpdate = startTime;
  }

  if (startTime - previousTimeStationList > 10000) {
    
#if NORMAL_MODE
    ap.getConnectedList();
#endif
    previousTimeStationList = startTime;
  }
}

boolean connectToNetwork(const char *hostname) {
#if NORMAL_MODE
Serial.printf("Konfiguracja: \n\r SSID: %s \n\r Password: %s", wifihandle.config.ssid,wifihandle.config.password);
  WiFi.mode(WIFI_AP);
  WiFi.setHostname("masterhorizon");
  esp_wifi_set_protocol(WIFI_IF_AP, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N);
 Serial.println(WiFi.softAPConfig(ap_local_IP, ap_gateway, ap_subnet)
                     ? "Configuring Soft AP"
                     : "Error in Configuration");
  esp_wifi_set_country(&wifi_country);
  WiFi.softAP(wifihandle.config.ssid, wifihandle.config.password);

  Serial.println(WiFi.softAPIP());
#if ENABLE_MDNS
  if (!MDNS.begin("esp32")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  MDNS.addService("http", "tcp", 80);
#endif
  return true;
#else

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifihandle.config.ssid, wifihandle.config.password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  WiFi.setHostname(hostname);

#if DEBUG_WIFI
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
#endif

#if ENABLE_MDNS
  if (!MDNS.begin("esp32")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  MDNS.addService("http", "tcp", 80);
#endif
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  } else {
    return false;
  }
#endif
}