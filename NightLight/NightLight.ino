// External libs

// Global vars

// Hand made logic
#include "jsonStateHelper.h"
JsonState State;
#include "wifi_init.h"
#include "http_init.h"
#include "ws_init.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.setDebugOutput(true);
    Serial.println("SETUP - RUN");

    // Получаю конфигурацию из файловой системы
    State.loadState();
//    Serial.println("FROM:");
//    State.printStateChar();
//    State.data.status = false;
    State.data.effect = random(10, 20);
    State.saveState();
    Serial.println("PRINT:");
    State.printStateChar();

    // Инициализирую WIFI
    WIFIinit();

    // Инициализирую веб сервер
    HTTP_init();

    // Инициализирую веб сокет сервер
    WS_init();
}

void loop() {
    dnsServer.processNextRequest();
    server.handleClient();
    webSocket.loop();
}
