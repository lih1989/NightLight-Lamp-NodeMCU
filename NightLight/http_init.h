ESP8266WebServer server(80);

String getContentType(String filename){
    if(server.hasArg("download")) return "application/octet-stream";
    else if(filename.endsWith(".htm")) return "text/html";
    else if(filename.endsWith(".html")) return "text/html";
    else if(filename.endsWith(".css")) return "text/css";
    else if(filename.endsWith(".svg")) return "image/svg+xml";
    else if(filename.endsWith(".woff")) return "application/font-woff";
    else if(filename.endsWith(".woff2")) return "application/font-woff2";
    else if(filename.endsWith(".ttf")) return "application/x-font-truetype";
    else if(filename.endsWith(".eot")) return "application/vnd.ms-fontobject";
    else if(filename.endsWith(".otf")) return "application/x-font-opentype";
    else if(filename.endsWith(".js")) return "application/javascript";
    else if(filename.endsWith(".png")) return "image/png";
    else if(filename.endsWith(".gif")) return "image/gif";
    else if(filename.endsWith(".jpg")) return "image/jpeg";
    else if(filename.endsWith(".ico")) return "image/x-icon";
    else if(filename.endsWith(".xml")) return "text/xml";
    else if(filename.endsWith(".pdf")) return "application/x-pdf";
    else if(filename.endsWith(".zip")) return "application/x-zip";
    else if(filename.endsWith(".json")) return "text/html";
    else if(filename.endsWith(".gz")) return "application/x-gzip";
    return "text/plain";
}

void handleNotFound()
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET)?"GET":"POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i=0; i<server.args(); i++){
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

bool handleFileRead(String path){
    Serial.println("handleFileRead: " + path);
    if(path.endsWith("/")) path += "index.html";
    String contentType = getContentType(path);
    String pathWithGz = path + ".gz";
    if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
        if(SPIFFS.exists(pathWithGz))
            path += ".gz";
        File file = SPIFFS.open(path, "r");
        size_t sent = server.streamFile(file, contentType);
        file.close();
        return true;
    }
    return false;
}

void HTTP_init(void) {
    server.on("/config.json", HTTP_GET, []() {
    server.send(200, "application/json", configSetup);
    });

    server.on("/", HTTP_GET, [](){
    if(!handleFileRead("/index.html")) server.send(404, "text/plain", "FileNotFound");
    });
  
//  server.on("/index.htm", HTTP_GET, [](){
//    if(!handleFileRead("/index.htm")) server.send(404, "text/plain", "FileNotFound");
//  });  //list directory
//
//  server.on("/settings.htm", HTTP_GET, [](){
//    if(!handleFileRead("/settings.htm")) server.send(404, "text/plain", "FileNotFound");
//  });
//
//  server.on("/sound.htm", HTTP_GET, [](){
//    if(!handleFileRead("/sound.htm")) server.send(404, "text/plain", "FileNotFound");
//  });
//
//  server.on("/settings", HTTP_GET, [](){
//    if(!handleFileRead("/settings.htm")) server.send(404, "text/plain", "FileNotFound");
//  });
//
//  server.on("/list", HTTP_GET, handleFileList);
//  //load editor
//  server.on("/edit", HTTP_GET, [](){
//    if(!handleFileRead("/edit.htm")) server.send(404, "text/plain", "FileNotFound");
//  });
  
//  server.on("/ssid", HTTP_GET, []() {
//      jsonWrite(configSetup, "ssid", server.arg("ssid"));
//      jsonWrite(configSetup, "password", server.arg("password"));
//      saveConfig();                 //    Flash
//      server.send(200, "text/plain", "OK");
//  });
   //    SSDP
//  server.on("/ssidap", HTTP_GET, []() {
//      jsonWrite(configSetup, "ssidAP", server.arg("ssidAP"));
//      jsonWrite(configSetup, "passwordAP", server.arg("passwordAP"));
//      saveConfig();                 //    Flash
//      server.send(200, "text/plain", "OK");
//  });
 
  
//  //create file
//  server.on("/edit", HTTP_PUT, handleFileCreate);
//  //delete file
//  server.on("/edit", HTTP_DELETE, handleFileDelete);
//  //first callback is called after the request has ended with all parsed arguments
//  //second callback handles file uploads at that location
//  server.on("/edit", HTTP_POST, [](){ server.send(200, "text/plain", ""); }, handleFileUpload);

    //called when the url is not defined here
    //use it to load content from SPIFFS
    server.onNotFound(handleNotFound);
    server.serveStatic("/img", SPIFFS, "/img","max-age=86400");
    server.serveStatic("/js",   SPIFFS, "/js"  ,"max-age=86400");
    server.serveStatic("/css",  SPIFFS, "/css" ,"max-age=86400");
  
  //get heap status, analog input value and all GPIO statuses in one json call
//  server.on("/all", HTTP_GET, [](){
//    String json = "{";
//    json += "\"heap\":"+String(ESP.getFreeHeap());
//    json += ", \"analog\":"+String(analogRead(A0));
//    json += ", \"gpio\":"+String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16)));
//    json += "}";
//    server.send(200, "text/json", json);
//    json = String();
//  });
    server.begin();
    Serial.println("HTTP server started");
}
