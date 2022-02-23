#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>

DNSServer dnsServer;
const IPAddress apIP(192, 168, 1, 1);
uint8_t connect = 0;

//	Запускаю DNS - нужен для автооткрытия браузера с веб страницей настроек
bool StartDnsServer(IPAddress apIP) {
    const byte DNS_PORT = 53;
    // if DNSServer is started with "*" for domain name, it will reply with
    // provided IP to all DNS request
    dnsServer.start(DNS_PORT, "*", apIP);
    Serial.println("DNS server started");
    return true;
}

bool StartAPMode() {
	// Отключаем WIFI
	WiFi.disconnect();
	// Меняем режим на режим точки доступа
	WiFi.mode(WIFI_AP);
	// Задаем настройки сети
	WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
	// Включаем WIFI в режиме точки доступа с именем и паролем
	// хронящихся в переменных _ssidAP _passwordAP
	String _ssidAP = Json.readPropertyByKey(configSetup, "ssidAP");
	String _passwordAP = Json.readPropertyByKey(configSetup, "passwordAP");

    //  Ограничение по кол-ву полключенных клиентов чтобы не было конфликтов при настройке
    //	ssid, pass, channel, hidden, max_connection
	WiFi.softAP(_ssidAP.c_str(), _passwordAP.c_str(), 3, false, 1);

    //	Запускаю DNS
    StartDnsServer(apIP);
	return true;
}

void WIFIinit() {
    Serial.println("Инициализация WIFI");
    // --------------------Получаем SSDP со страницы
    Serial.println("Подключение к станции...");

    // Попытка подключения к точке доступа
    WiFi.mode(WIFI_STA);
    byte tries = 11;
    String _ssid = Json.readPropertyByKey(configSetup, "ssid");
    String _password = Json.readPropertyByKey(configSetup, "password");

    if (_ssid == "" && _password == "") {
        WiFi.begin();
    } else {
        WiFi.begin(_ssid.c_str(), _password.c_str());
    }

    // Делаем проверку подключения до тех пор пока счетчик tries
    // не станет равен нулю или не получим подключение
    while (--tries && WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }

    if (WiFi.status() != WL_CONNECTED) {
        // Если не удалось подключиться к станции запускаем в режиме AP
        Serial.println("WIFI в режиме Access Point (AP)");
        Serial.println("");
        Serial.println("WiFi up AP");
        StartAPMode();
        connect = 0;
        IPAddress myIP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(myIP);
    } else {
        Serial.println("WIFI в режиме Station (STA)");
        // Иначе удалось подключиться отправляем сообщение
        // о подключении и выводим адрес IP
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        connect = 1;
    }
}