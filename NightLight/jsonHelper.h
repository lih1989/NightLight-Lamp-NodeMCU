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
    // Обработчик сообщений от клиента
    String wsJsonPayloadHandler (uint8_t * payload) {
        Serial.println("wsJsonPayloadHandler");
        const uint8_t size = JSON_OBJECT_SIZE(2);
        StaticJsonDocument<size> json;
        DeserializationError err = deserializeJson(json, payload);
        if (err) {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(err.c_str());
            String error = "deserializeJson() failed with code: " + String(err.c_str()) + ".";
            return "{\"error\":" + error + "}";
        }

        const char *action = json["action"]; // действия  get, set
        const char *data = json["data"]; // набор данных которые нужно записать
        Serial.println(action);
        Serial.println(data);
//        if (strcmp(action, "toggle") == 0) {
//            led.on = !led.on;
//            notifyClients();
//        }
        return "";
    }
};
