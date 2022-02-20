/*
  TODO
*/
// global libs
#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// hand made libs
#include "json.h"
#include "wifi_init.h"
#include "http_init.h"

void setup() {
    Serial.begin(115200);
    Serial.println("SETUP - RUN");

    SPIFFS.begin();
    // Конфиг из Файловой системы
    configSetup = readFile("config.json", 4096);
    Serial.println(configSetup);

    // Разворачивание WIFI
    WIFIinit();

    // Разворачивание веб сервера
    HTTP_init();
}

void loop() {
    server.handleClient();
}
