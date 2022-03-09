#include <ArduinoJson.h>
#include <FS.h>

struct State {
  bool status;
  bool motionSensor;
  long motionSensorSeconds;
  int effect;
  char ssidAP[64];
  char passwordAP[64];
  char ssid[64];
  char password[64];
  int volume;
};

class JsonState {
  public:
    State data;

    JsonState () {
      SPIFFS.begin();
    };

    // Инициализировать состояние хранилища из файла
    void initReadState (const char* stateFilePath) {
      // Open file for reading
      File file = SPIFFS.open(stateFilePath, "r");
      if (!file) {
        Serial.println(F("loadConfiguration - Failed to open file"));
        return;
      }

      // Allocate a temporary JsonDocument
      // Don't forget to change the capacity to match your requirements.
      // Use https://arduinojson.org/v6/assistant to compute the capacity.
      StaticJsonDocument<512> doc;

      // Deserialize the JSON document
      DeserializationError error = deserializeJson(doc, file);
      if (error){
        Serial.println(F("loadConfiguration - deserializeJson: Failed to read file, using default configuration"));
        return;
      }

      // Copy values from the JsonDocument to the State
      data.status = doc["status"] | false;
      data.effect = doc["effect"] | 1;
      data.volume = doc["volume"] | 10;
      strlcpy(data.ssidAP, doc["ssidAP"], sizeof(data.ssidAP));
      strlcpy(data.passwordAP, doc["passwordAP"], sizeof(data.passwordAP));
      strlcpy(data.ssid, doc["ssid"], sizeof(data.ssid));
      strlcpy(data.password, doc["password"], sizeof(data.password));
      // Serial.printf("LOAD: [data.ssidAP:%s], [doc[ssidAP]:%s]\n", data.ssidAP,  doc["ssidAP"]);

      // Close the file (Curiously, File's destructor doesn't close the file)
      file.close();
    }

    // Сохранить состояние хранилища в файл
    void writeFileState (const char* stateFilePath) {
      // Serial.println("--- writeFileState---START---");
      // Serial.printf("[ssidAP:%s], [passwordAP:%s], [ssid:%s], [password:%s]\n", data.ssidAP,  data.passwordAP, data.ssid, data.password);

      StaticJsonDocument<512> doc;

      // Set the values in the document
      doc["status"] = data.status;
      doc["effect"] = data.effect;
      doc["volume"] = data.volume;
      doc["ssidAP"] = data.ssidAP;
      doc["passwordAP"] = data.passwordAP;
      doc["ssid"] = data.ssid;
      doc["password"] = data.password;

      // Serialize JSON to file
      char strData[512];
      if (serializeJsonPretty(doc, strData) == 0) {
        Serial.println(F("writeFileState - serializeJsonPretty: Failed to write to file"));
      }
      // Serial.println("+++writeFileState - strData+++");
      // Serial.println(strData);


      Serial.printf("Writing file: %s\r\n", stateFilePath);

      File file = SPIFFS.open(stateFilePath, "w");
      if(!file){
          Serial.println("- failed to open file for writing");
          return;
      }
      if(file.print(strData)){
          Serial.println("- file written");
      } else {
          Serial.println("- write failed");
      }
      file.close();
      Serial.println("--- writeFileState---END---");
    }

    // метод читает состояние с файловой системы
    void readFileState (const char* stateFilePath, char* return_data) {
        // Serial.printf("Reading file: %s\n", stateFilePath);
        File file = SPIFFS.open(stateFilePath, "r");
        if (!file || file.isDirectory()) {
            Serial.println("Failed to open file for reading");
            return;
        }
        // Serial.print("Read from file: ");
        char tmp[file.size() + 1];
        int n=0;
        while (file.available()) {
            char c =  file.read();
            delayMicroseconds(100);
            tmp[n] = c; //записываю во временный буфер
            n=n+1;
        }
        file.close();

        strlcpy(return_data, tmp, sizeof(tmp));
    }

    // метод читает состояние из структуры
    void getState (char* output) {
      StaticJsonDocument<512> doc;

      // Set the values in the document
      doc["status"] = data.status;
      doc["effect"] = data.effect;
      doc["volume"] = data.volume;
      doc["ssidAP"] = data.ssidAP;
      doc["passwordAP"] = data.passwordAP;
      doc["ssid"] = data.ssid;
      doc["password"] = data.password;

      char tmp[512];
      if (serializeJsonPretty(doc, tmp) == 0) {
        Serial.println(F("getState - serializeJsonPretty: Failed to write to file"));
      }
      strlcpy(output, tmp, sizeof(tmp));
    }

    // Изменить состояние хранилища в соотвествии с данными веб сокетов
    void wsJsonPayloadHandler(char * payload) {
      // Serial.println(payload);
      StaticJsonDocument<512> doc;
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.println(F("wsJsonPayloadHandler - wsJsonPayloadHandler: Failed to read"));
        return;
      }

      if (doc.containsKey("status")) {
        data.status = doc["status"];
      }

      if (doc.containsKey("effect")) {
        data.effect = doc["effect"];
      }

      if (doc.containsKey("volume")) {
        data.volume = doc["volume"];
      }

      if (doc.containsKey("ssidAP")) {
        strlcpy(data.ssidAP, doc["ssidAP"], sizeof(data.ssidAP));
      }

      if (doc.containsKey("passwordAP")) {
        strlcpy(data.passwordAP, doc["passwordAP"], sizeof(data.passwordAP));
      }

      if (doc.containsKey("ssid")) {
        strlcpy(data.ssid, doc["ssid"], sizeof(data.ssid));
      }

      if (doc.containsKey("password")) {
        strlcpy(data.password, doc["password"], sizeof(data.password));
      }
    }
};
