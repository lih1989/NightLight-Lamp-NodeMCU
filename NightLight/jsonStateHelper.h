#include <ArduinoJson.h>
#include <FS.h>
//{
//  "status": true,
//  "effect": 1,
//  "ssidAP": "NightLamp",
//  "passwordAP": "todo_dev",
//  "ssid": "dd-wrt",
//  "password": "qwerty12345",
//  "volume": 10
//}
struct State {
  bool status;
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
    JsonState(){
      SPIFFS.begin();
    };
    const char *stateFilePath = "/config.json";

    // Loads the configuration from a file
    void loadState() {
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

      // Copy values from the JsonDocument to the Config
      data.status = doc["status"] | false;
      data.effect = doc["effect"] | 1;
      data.volume = doc["volume"] | 10;
      strlcpy(data.ssidAP, doc["ssidAP"], sizeof(data.ssidAP));
      strlcpy(data.passwordAP, doc["passwordAP"], sizeof(data.passwordAP));
      strlcpy(data.ssid, doc["ssid"], sizeof(data.ssid));
      strlcpy(data.password, doc["password"], sizeof(data.password));

      // Close the file (Curiously, File's destructor doesn't close the file)
      file.close();
    }

    // Saves the configuration to a file
    void saveState() {
      Serial.println("--- saveState---");
//      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
//      Serial.printf("[volume:%d], [status:%s], [effect:%s]\n", data.volume, data.status, data.effect);
      Serial.printf("[ssidAP:%s], [passwordAP:%s], [ssid:%s], [password:%s]\n", data.ssidAP,  data.passwordAP, data.ssid, data.password);
      // Delete existing file, otherwise the configuration is appended to the file
//      File file = SPIFFS.open(stateFilePath, "w");
//      if (!file) {
//        Serial.println(F("saveState - Failed to open file"));
//      }

      StaticJsonDocument<512> doc;
//
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
        Serial.println(F("saveState - serializeJsonPretty: Failed to write to file"));
      }
        Serial.println("+++saveState - strData+++");
        Serial.println(strData);

//      int bytesWritten = file.print(file);
//      if (bytesWritten > 0) {
//        Serial.println("File was written");
//        Serial.println(bytesWritten);
//
//      } else {
//        Serial.println("File write failed");
//      }
//
//      // Close the file
//      file.close();
    }

    // Prints the content of a file to the Serial
    char * printStateChar() {
      File file = SPIFFS.open(stateFilePath, "r");
      if (!file) {
        Serial.println(F("printFile - Failed to open file"));
      }

      size_t fileSize = file.size();
      char fileCharArray[fileSize];
      file.readBytes(fileCharArray, fileSize);

      file.close();
      Serial.println(fileCharArray);
      return fileCharArray;
    }
    // Prints the content of a file to the Serial
    char * wsJsonPayloadHandler(char * payload) {
      Serial.println(payload);

      StaticJsonDocument<512> doc;
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.println(F("wsJsonPayloadHandler - wsJsonPayloadHandler: Failed to read"));
        return printStateChar();
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
      saveState();
      return printStateChar();
    }

};
