// External libs

// Global vars

// Hand made logic
#include "jsonStateHelper.h"
JsonState ConfigState;
char stateFilePath[] = "/config.json";
#include "wifi_init.h"
// #include "http_init.h" TODO OLD
#include "AsyncHttpInit.h"
#include "ws_init.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.setDebugOutput(true);
    Serial.println("SETUP - RUN");

    // Получаю конфигурацию из файловой системы
    ConfigState.initReadState(stateFilePath);
    // TODO RM DEBUG
    ConfigState.data.effect = random(10, 20);

    ConfigState.writeFileState(stateFilePath);

    char tmp[256];
    Serial.println("SETUP PRINT:");
    ConfigState.getState(tmp);
    Serial.println(tmp);

    // Инициализирую WIFI
    WIFIinit();

    // Инициализирую веб сервер
//     HTTP_init(); TODO OLD
    initHttp();

    // Инициализирую веб сокет сервер
    WS_init();
}

void loop() {
    dnsServer.processNextRequest();
//     server.handleClient();
    webSocket.loop();
}
