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

    void readFile123(char* return_data) {
        int n=0;
        Serial.printf("Reading file: %s\n", stateFilePath);
    
        File file = SPIFFS.open(stateFilePath, "r");
        if (!file || file.isDirectory())
        {
            Serial.println("Failed to open file for reading");
            return;
        }
        Serial.print("Read from file: ");
        char tmp[file.size() + 1];
        while (file.available()) {
            char c =  file.read();
            delayMicroseconds(100);
    //        Serial.print(c);
    //        strcat(return_data, &c); //returns meditation error
    //        return_data[n]=c; //returns meditation error
            tmp[n] = c; //записываю во временный буфер
            n=n+1;
        }
        file.close();
    
        strlcpy(return_data, tmp, sizeof(tmp));
    }


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
      Serial.printf("LOAD: [data.ssidAP:%s], [doc[ssidAP]:%s]\n", data.ssidAP,  doc["ssidAP"]);
      strlcpy(data.passwordAP, doc["passwordAP"], sizeof(data.passwordAP));
      strlcpy(data.ssid, doc["ssid"], sizeof(data.ssid));
      strlcpy(data.password, doc["password"], sizeof(data.password));

      // Close the file (Curiously, File's destructor doesn't close the file)
      file.close();
    }

    // Saves the configuration to a file
    void saveState() {
      Serial.println("--- saveState---");
      Serial.printf("[ssidAP:%s], [passwordAP:%s], [ssid:%s], [password:%s]\n", data.ssidAP,  data.passwordAP, data.ssid, data.password);

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
        Serial.println(F("saveState - serializeJsonPretty: Failed to write to file"));
      }
      Serial.println("+++saveState - strData+++");
      Serial.println(strData);


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
    }

    // Prints the content of a file to the Serial
    char * printStateChar() {
      Serial.println("--- printStateChar ---");
      int n=0;
      File file = SPIFFS.open(stateFilePath, "r");
      if(!file || file.isDirectory()){
          Serial.println("- failed to open file for reading");
      }

//      size_t fileSize = file.size();
//      char fileCharArray[fileSize];
//      file.read((uint8_t *)fileCharArray, sizeof(fileCharArray));
//      file.close();
      char tmp[file.size() + 1];
      while (file.available()) {
          char c =  file.read();
          delayMicroseconds(100);
          tmp[n] = c; //записываю во временный буфер
          n=n+1;
      }
      file.close();
      Serial.println(tmp);
      Serial.println("--- END ---");
      return tmp;
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
