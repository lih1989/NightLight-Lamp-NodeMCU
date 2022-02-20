/*
  TODO
*/
// global libs
#include <FS.h>
#include <ESP8266WiFi.h>

// hand made libs
#include "json.h"
#include "wifi_init.h"

void setup() {
    Serial.begin(115200);
    Serial.println("SETUP - RUN");

    SPIFFS.begin();
    // Конфиг из Файловой системы
    configSetup = readFile("config.json", 4096);
    Serial.println(configSetup);

    // Разворачивание WIFI
    WIFIinit();
}

void loop() {}
