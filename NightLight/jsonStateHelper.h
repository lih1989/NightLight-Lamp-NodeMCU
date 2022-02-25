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
  const char* ssidAP;
  const char* passwordAP;
  const char* ssid;
  const char* password;
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
      }
    
      // Allocate a temporary JsonDocument
      // Don't forget to change the capacity to match your requirements.
      // Use https://arduinojson.org/v6/assistant to compute the capacity.
      StaticJsonDocument<512> doc;
    
      // Deserialize the JSON document
      DeserializationError error = deserializeJson(doc, file);
      if (error)
        Serial.println(F("loadConfiguration - deserializeJson: Failed to read file, using default configuration"));
    
      // Copy values from the JsonDocument to the Config
      data.status = doc["status"] | false;
      data.effect = doc["effect"] | 1;
      data.volume = doc["volume"] | 10;
      data.ssidAP = doc["ssidAP"];
      data.passwordAP = doc["passwordAP"];
      data.ssid = doc["ssid"];
      data.password = doc["password"];
    
      // Close the file (Curiously, File's destructor doesn't close the file)
      file.close();
    }
    
    // Saves the configuration to a file
    void saveState() {
      // Delete existing file, otherwise the configuration is appended to the file
      File file = SPIFFS.open(stateFilePath, "w");
      if (!file) {
        Serial.println(F("saveConfiguration - Failed to open file"));
      }
    
      StaticJsonDocument<256> doc;
    
      // Set the values in the document
      doc["status"] = data.status;
      doc["effect"] = data.effect;
      doc["volume"] = data.volume;
      doc["ssidAP"] = data.ssidAP;
      doc["passwordAP"] = data.passwordAP;
      doc["ssid"] = data.ssid;
      doc["password"] = data.password;
    
      // Serialize JSON to file
      if (serializeJson(doc, file) == 0) {
        Serial.println(F("saveConfiguration - serializeJson: Failed to write to file"));
      }
    
      int bytesWritten = file.print(file);
      if (bytesWritten > 0) {
        Serial.println("File was written");
        Serial.println(bytesWritten);
     
      } else {
        Serial.println("File write failed");
      }
      
      // Close the file
      file.close();
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
        Serial.println(F("loadConfiguration - wsJsonPayloadHandler: Failed to read"));
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
        data.ssidAP = doc["ssidAP"];
      }

      if (doc.containsKey("passwordAP")) {
        data.passwordAP = doc["passwordAP"];
      }

      if (doc.containsKey("ssid")) {
        data.ssid = doc["ssid"];
      }

      if (doc.containsKey("password")) {
        data.password = doc["password"];
      }
      saveState();
      return printStateChar();
    }

};
