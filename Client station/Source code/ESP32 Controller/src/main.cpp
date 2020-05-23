/*
 * Copyright  Michał Młodawski (SimpleMethod)(c) 2019.
 */
#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <esp_wifi.h>
#include "ESPAsyncWebServer.h"
#include <string>
#include <HTTPClient.h>
#include "SPIFFS.h"
#include "FS.h"
#include "SPIFFShandle.hpp"
#include "WiFihandle.hpp"
#include "Uarttransaction.hpp"
#include "LTC4151.h"
#include <Wire.h>
#include "SparkFunBME280.h"

#define FORMAT_SPIFFS_IF_FAILED true
#define DEBUG_HTTP_REQUEST true
#define DEBUG_WIFI false
#define FILE_DIR false
#define ENABLE_MDNS true

BME280 bme280a;
AsyncWebServer server(80);
spiffsfs_handle sph;
wifi_handle wifihandle(sph);
LTC4151 sensor;

unsigned long startTime = 0;
unsigned long previousTimeUptime = 0;
unsigned long previousTimeUpdate = 0;

StaticJsonDocument<512> jsonData;
String jsonParserData, temp_jsonParserData;

const char *hostname = "StacjaHorizon153b949912fb4795";

boolean connectToNetwork(const char *hostname);
String wind_averageValue(int ADC_PIN);
int moisture_avergeValue(int ADC_PIN);
#if FILE_DIR
void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
#endif

void setup(void)
{
    Wire.begin(21, 23);
    bme280a.setI2CAddress(0x76);
    sensor.init(LTC4151::L, LTC4151::L);
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, 18, 19);
    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    if (sph.checkFile("/wifi.bin"))
    {
        sph.writeFile("/wifi.bin", "{ \"ssid\":\"horizon\", \"password\": \"horizon2137\",  \"refreshtime\": \"10000\"}");
    }

    wifihandle.loadConfiguration();
    delay(500);
    connectToNetwork(hostname);

#if ENABLE_MDNS
    if (!MDNS.begin("esp32"))
    {
        Serial.println("Error setting up MDNS responder!");
        while (1)
        {
            delay(1000);
        }
    }
    Serial.println("mDNS responder started");
    MDNS.addService("http", "tcp", 80);
#endif

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/index.html"); });
    server.on("/connection", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/wifi.bin"); });
    server.on("/fonts/icomoon.eot", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/fonts/icomoon.eot"); });
    server.on("/fonts/icomoon.svg", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/fonts/icomoon.svg"); });
    server.on("/fonts/icomoon.ttf", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/fonts/icomoon.ttf"); });
    server.on("/fonts/icomoon.woff", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/fonts/icomoon.woff"); });
    server.on("/angular.min.js", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/angular.min.js"); });
    server.on("/angular-route.js", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/angular-route.js"); });
    server.on("/app.js", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/app.js"); });
    server.on("/aws.png", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/aws.png"); });
    server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/bootstrap.min.css"); });
    server.on("/home.html", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/home.html"); });
    server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/index.html"); });
    server.on("/jquery.easing.js", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/jquery.easing.js"); });
    server.on("/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/jquery.min.js"); });
    server.on("/settings.html", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/settings.html"); });
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/style.css"); });
    server.on("/theme.js", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/theme.js"); });

    server.on("/horizon/connection/settings", HTTP_POST, [](AsyncWebServerRequest *request) {
        const char *ssid = 0;
        const char *password = 0;
        const char *refreshtime = 0;

        int temp = 0;
        if (request->hasParam("ssid"))
        {
            AsyncWebParameter *p = request->getParam("ssid");
            ssid = p->value().c_str();
            temp++;
            if (p->value().isEmpty())
            {
                request->send(404, "text/html", "error");
            }
        }
        if (request->hasParam("password"))
        {
            AsyncWebParameter *p = request->getParam("password");
            password = p->value().c_str();
            temp++;
            if (p->value().isEmpty())
            {
                request->send(404, "text/html", "error");
            }
        }
        if (request->hasParam("refreshtime"))
        {
            AsyncWebParameter *p = request->getParam("refreshtime");
            refreshtime = p->value().c_str();
            temp++;
            if (p->value().isEmpty())
            {
                request->send(404, "text/html", "error");
            }
        }
        if (temp >= 2)
        {
            wifihandle.saveConfiguration(ssid, password, refreshtime);
            request->send(200, "text/html", "ok");
        }
        else
        {
            request->send(500, "text/html", "error");
        }
    });

    server.on("/horizon/connectionstatus", HTTP_GET, [](AsyncWebServerRequest *request) {
        wifihandle.setWifiStatus(WiFi.SSID(), WiFi.localIP().toString(), WiFi.RSSI(), WiFi.getHostname(), wifihandle.getTimer());
        request->send(200, "application/json", wifihandle.getConnectionStatus());
    });
    server.on("/horizon/sensors/data", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", temp_jsonParserData);
    });
    server.on("/horizon/connection/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/wifi.bin");
    });

    server.begin();
    Serial.println("HTTP server started");

#if FILE_DIR
    listDir(SPIFFS, "/", 0);
#endif

    pinMode(27, OUTPUT);
    pinMode(14, OUTPUT);
    pinMode(12, OUTPUT);
    digitalWrite(27, HIGH);
    digitalWrite(14, HIGH);
    digitalWrite(12, HIGH);
}

void loop()
{
    startTime = millis();

    if (startTime - previousTimeUptime > 2000)
    {
        jsonParserData.clear();
        int64_t tempTime = wifihandle.getTimer();
        tempTime += 1;
        previousTimeUptime = startTime;
        wifihandle.setWifiStatus(WiFi.SSID(), WiFi.localIP().toString(), WiFi.RSSI(), WiFi.getHostname(), tempTime);

        wifi_ap_record_t wifidata;

        if (bme280a.beginI2C() == false)
        {
            Serial.println("Sensor A connect failed");
        }
        Serial2.print(2);
        Serial2.flush();
        delay(100);
        String pm25 = Serial2.readStringUntil(10);
        delay(10);
        Serial2.print(3);
        Serial2.flush();
        delay(100);
        String pm10 = Serial2.readStringUntil(10);
        delay(10);

        String wind_direction = wind_averageValue(34);
        String wind_direction_temp;

        if (!wind_direction.isEmpty() || wind_direction != "" || wind_direction != NULL)
        {
            wind_direction_temp = wind_direction;
        }
        int moisture = moisture_avergeValue(35);
        jsonData["horizon_id"] = "153b949912fb4795";
        jsonData["air_temperature"] = bme280a.readTempC();
        jsonData["air_humidity"] = bme280a.readFloatHumidity();
        jsonData["atmospheric_pressure"] = bme280a.readFloatPressure();
        jsonData["soil_moisture"] = moisture;
        jsonData["fine_particles25"] = pm25.c_str();
        jsonData["fine_particles100"] = pm10.c_str();
        jsonData["wind_direction"] = wind_direction_temp.c_str();
        jsonData["current_load"] = sensor.getSnapshotLoadCurrent(0.1);
        jsonData["battery_voltage"] = sensor.getSnapshotInputVoltage();
        //jsonData["solar_charging"] = 0;
        if (esp_wifi_sta_get_ap_info(&wifidata) == 0)
        {
            jsonData["rssi"] = wifidata.rssi;
        }
        serializeJson(jsonData, jsonParserData);
        temp_jsonParserData = jsonParserData;
    }

    if (startTime - previousTimeUpdate > atoi(wifihandle.config.refreshtime))
    {

        previousTimeUpdate = startTime;
#define HTTP_ENABLE true
#if HTTP_ENABLE
        int httpResponseCode;
        if (WiFi.status() == WL_CONNECTED)
        {
            HTTPClient http;
            http.setTimeout(300);

            http.addHeader("Content-Type", "application/json");
            http.begin("http://192.168.1.1/upload/data");

#if DEBUG_HTTP_REQUEST
            Serial.println("---------------------------------------------------");
            httpResponseCode = http.POST(temp_jsonParserData);
            Serial.println(httpResponseCode);
            Serial.println("---------------------------------------------------");
#else
            httpResponseCode = http.POST(temp_jsonParserData);
#endif

            http.end();
        }
        else
        {
            connectToNetwork(hostname);
            Serial.println("Proba polaczenia");
        }
#endif
    }
}

#if FILE_DIR
void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        Serial.println("- failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels)
            {
                listDir(fs, file.name(), levels - 1);
            }
        }
        else
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}
#endif

boolean connectToNetwork(const char *hostname)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifihandle.config.ssid, wifihandle.config.password);
    delay(300);
    delay(300);
    Serial.println("");

    while (WiFi.status() != WL_CONNECTED)
    {
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

    if (WiFi.status() == WL_CONNECTED)
    {
        return true;
    }
    else
    {
        return false;
    }
}

String wind_averageValue(int ADC_PIN)
{
    uint16_t analog_pin = analogRead(ADC_PIN);
    if ((analog_pin >= 318 && analog_pin <= 319) || (analog_pin >= 721 && analog_pin <= 766) || (analog_pin >= 292 && analog_pin <= 299))
    {
        return "NW";
    }
    else if ((analog_pin >= 1240 && analog_pin <= 1295) || (analog_pin >= 933 && analog_pin <= 951) || (analog_pin >= 960 && analog_pin <= 988))
    {
        return "NE";
    }

    if ((analog_pin >= 643 && analog_pin <= 689) || (analog_pin >= 272 && analog_pin <= 299))
    {
        return "SE";
    }
    else if ((analog_pin >= 321 && analog_pin <= 333) || (analog_pin >= 523 && analog_pin <= 592) || (analog_pin >= 234 && analog_pin <= 270))
    {
        return "SW";
    }

    if ((analog_pin >= 309 && analog_pin <= 315) || (analog_pin >= 473 && analog_pin <= 501) || (analog_pin >= 209 && analog_pin <= 230))
    {
        return "S";
    }
    else if ((analog_pin >= 361 && analog_pin <= 402) || (analog_pin >= 299 && analog_pin <= 308) || (analog_pin >= 315 && analog_pin <= 320))
    {
        return "W";
    }
    else if ((analog_pin >= 1377 && analog_pin <= 1430) || (analog_pin >= 1110 && analog_pin <= 1142) || (analog_pin >= 701 && analog_pin <= 721))
    {
        return "N";
    }
    else if ((analog_pin >= 1473 && analog_pin <= 1492) || (analog_pin >= 1436 && analog_pin <= 1472) || (analog_pin >= 1499 && analog_pin <= 1528))
    {
        return "E";
    }
}

int moisture_avergeValue(int ADC_PIN)
{
    uint16_t analog_pin = analogRead(ADC_PIN);

    if (analog_pin < 1500)
    {
        return 2;
    }
    else if (analog_pin >= 1501 && analog_pin <= 2500)
    {
        return 1;
    }
    else if (analog_pin >= 2501)
    {
        return 0;
    }
}
