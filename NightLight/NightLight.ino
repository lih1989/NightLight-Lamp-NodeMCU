// External libs
#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Global vars
String configSetup = "{}";

// Hand made logic
#include "fileSystemHelper.h"
FileSystemHelper FileSystem;
#include "jsonHelper.h"
JsonHelper Json;
#include "wifi_init.h"
#include "http_init.h"
#include "ws_init.h"

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println("SETUP - RUN");

    // Получаю конфигурацию из файловой системы
    configSetup = FileSystem.readFile("config.json", 4096);
    Serial.println(configSetup);

    // Инициализирую WIFI
    WIFIinit();

    // Инициализирую веб сервер
    HTTP_init();

    // Инициализирую веб сокет сервер
    WS_init();
}

void loop() {
    server.handleClient();
    webSocket.loop();
}
