#include <WebSocketsServer.h>
#include <Hash.h>

WebSocketsServer webSocket = WebSocketsServer(1181);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
                // TODO DEV
                char tmp[256];
                ConfigState.getState(tmp);
                Serial.println("WStype_CONNECTED PRINT:");
                Serial.println(tmp);
                // send message to client
                webSocket.sendTXT(num, tmp);
            break;
        }
        case WStype_TEXT: {
            Serial.printf("[%u] get Text: %s\n", num, payload);
            // TODO DEV
            //  Обрабатываю входящее сообщение и генерирую ответ
            ConfigState.wsJsonPayloadHandler((char*) payload);
            char tmp[256];
            ConfigState.getState(tmp);
            Serial.println("WStype_TEXT PRINT:");
            Serial.println(tmp);
            // send message to client
            webSocket.sendTXT(num, tmp);
            ConfigState.writeFileState(stateFilePath);

            // send message to client
            // webSocket.sendTXT(num, "message here");
            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            break;
        }
        case WStype_BIN: {
            Serial.printf("[%u] get binary length: %u\n", num, length);
            hexdump(payload, length);

            // send message to client
            // webSocket.sendBIN(num, payload, length);
            break;
        }
        default: {
            Serial.printf("SWITCH DEFAULT: webSocketEvent(%d, %d, ...)\r\n", num, type);
            break;
        }
    }

}

void WS_init() {
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}
