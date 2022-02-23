#include <ArduinoJson.h>

class JsonHelper {
  public:
    String readPropertyByKey(String &jsonString, String key, int maxSize = 1024) {     // Получить значение свойство по ключу
        Serial.println("StrData: [" + key + "] - " + jsonString);
        DynamicJsonDocument doc(maxSize);
        deserializeJson(doc, jsonString);
        JsonObject rootJsonObj = doc.as<JsonObject>();
        String output = rootJsonObj[key].as<String>();
        Serial.println("readPropertyByKey: [" + key + "] = " + output);
        return output;
    }
};
