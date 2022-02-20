/*
  TODO
*/
// global libs
#include <FS.h>

// hand made libs
#include "json.h"

void setup() {
    Serial.begin(115200);
    Serial.println("SETUP - RUN");
    SPIFFS.begin();
    configSetup = readFile("config.json", 4096);
    Serial.println(configSetup);
}

void loop() {}
