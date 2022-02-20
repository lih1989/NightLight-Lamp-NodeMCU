#include <ArduinoJson.h>

String configSetup = "{}";
String configJson = "{}";

// Чтение файла в строку
String readFile(String fileName, size_t len ) {
    File configFile = SPIFFS.open("/" + fileName, "r");
    if (!configFile) {
        return "Failed to open config file";
    }
    size_t size = configFile.size();
    if (size > len) {
        configFile.close();
        return "Large";
    }
    String temp = configFile.readString();
    configFile.close();
    return temp;
}

// Чтение значения json из строки
String jsonRead(String &json, String name) {
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);
  JsonObject root = doc.as<JsonObject>();
  String result = root[name].as<String>();
  Serial.println("jsonRead: [" + name + "] = " + result);
  return result;
}

//// ------------- Чтение значения json
//int jsonReadtoInt(String &json, String name) {
//  DynamicJsonBuffer jsonBuffer;
//  JsonObject root = jsonBuffer.parseObject(json);
//  return root[name];
//}
//
//// ------------- Запись значения json String
//String jsonWrite(String &json, String name, String volume) {
//  DynamicJsonBuffer jsonBuffer;
//  JsonObject root = jsonBuffer.parseObject(json);
//  root[name] = volume;
//  json = "";
//  root.printTo(json);
//  return json;
//}
//
//// ------------- Запись значения json int
//String jsonWrite(String &json, String name, int volume) {
//  DynamicJsonBuffer jsonBuffer;
//  JsonObject root = jsonBuffer.parseObject(json);
//  root[name] = volume;
//  json = "";
//  root.printTo(json);
//  return json;
//}
//
//
//
//// ------------- Запись строки в файл
//String writeFile(String fileName, String strings ) {
//  File configFile = SPIFFS.open("/" + fileName, "w");
//  if (!configFile) {
//    return "Failed to open config file";
//  }
//  configFile.print(strings);
//  //strings.printTo(configFile);
//  configFile.close();
//  return "Write sucsses";
//}
//
//void saveConfig (){
//  writeFile("config.json", configSetup );
//}