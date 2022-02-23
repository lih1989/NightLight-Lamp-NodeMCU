#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

// Шаблон для редиректа
const String metaRefreshStr = "<head><meta http-equiv=\"refresh\" content=\"0; url=http://" + apIP.toString() + "\" /></head><body><p>redirecting...</p></body>";

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

bool handleFileRead(String path){
    // Serial.println("handleFileRead: " + path);
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

void handleNotFound()
{
    // Если host обращения не сщщтвествует ip адресу устройства
    if(apIP.toString() != String(server.hostHeader())) {
        Serial.println("---REDIRECT---");
        server.send(200, "text/html", metaRefreshStr);
    } else if(!handleFileRead(server.uri())) {
        Serial.println("---File Not Found---");
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
}

void HTTP_init(void) {
    //called when the url is not defined here
    //use it to load content from SPIFFS
    server.onNotFound(handleNotFound);
    server.serveStatic("/img", SPIFFS, "/img","max-age=86400");
    server.serveStatic("/js",   SPIFFS, "/js"  ,"max-age=86400");
    server.serveStatic("/css",  SPIFFS, "/css" ,"max-age=86400");

    server.begin();
    Serial.println("HTTP server started");
}
