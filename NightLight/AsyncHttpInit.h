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

// Шаблон для редиректа
const String metaRefreshStr = "<head><meta http-equiv=\"refresh\" content=\"0; url=http://" + apIP.toString() + "\" /></head><body><p>redirecting...</p></body>";

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    Serial.println("---canHandle---");
    Serial.println(apIP.toString());
    Serial.println(String(request->host()));
    //request->addInterestingHeader("ANY");
    if(apIP.toString() != String(request->host())) {
        return true;
    } else {
        return false;
    }
  }

  void handleRequest(AsyncWebServerRequest *request) {
    Serial.println("---CaptiveRequestHandler---");
    Serial.println(apIP.toString());
    Serial.println(String(request->host()));
    request->send(200, "text/html", metaRefreshStr);
  }
};

void notFound(AsyncWebServerRequest *request) {
    Serial.println("---notFound---");
    Serial.println(apIP.toString());
    Serial.println(String(request->host()));
    request->send(404, "text/plain", "Not found");
}

void initHttp(void) {
    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html"); //set AUTH .setAuthentication("user", "pass");
    // Serve files in directory "/www/" when request url starts with "/"
    // Request to the root or none existing files will try to server the default
    // file name "index.htm" if exists
    // [WARNING] serveStatic on '/' responds with net::ERR_ABORTED 404 for subdirectories
    // FIX: Maximum 5 requests at a time!
    server.serveStatic("/img/", SPIFFS, "/img/").setCacheControl("max-age=360");
    server.serveStatic("/js/", SPIFFS, "/js/").setCacheControl("max-age=360");
    server.serveStatic("/css/", SPIFFS, "/css/").setCacheControl("max-age=360");

    server.onNotFound(notFound);
    server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);//only when requested from AP

    server.begin();
    Serial.println("Async HTTP server started");
}
