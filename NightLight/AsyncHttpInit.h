//
// A simple server implementation showing how to:
//  * serve static messages
//  * read GET and POST parameters
//  * handle missing pages / 404s
//

#ifdef ESP32
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

const char* PARAM_MESSAGE = "message";

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void initHttp(void) {
    // Server with different default file
    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    //AND AUTH server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html").setAuthentication("user", "pass");

    // Serve files in directory "/www/" when request url starts with "/"
    // Request to the root or none existing files will try to server the defualt
    // file name "index.htm" if exists

    //  serveStatic on '/' responds with net::ERR_ABORTED 404 for subdirectories
    // FIX: Maximum 5 requests at a time!
    server.serveStatic("/img/", SPIFFS, "/img/").setCacheControl("max-age=360");
    server.serveStatic("/js/", SPIFFS, "/js/").setCacheControl("max-age=360");
    server.serveStatic("/css/", SPIFFS, "/css/").setCacheControl("max-age=360");

    server.onNotFound(notFound);

    server.begin();
    Serial.println("Async HTTP server started");
}
