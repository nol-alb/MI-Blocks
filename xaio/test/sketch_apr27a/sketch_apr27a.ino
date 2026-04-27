#include <ArduinoOSCWiFi.h>

const char* ssid = "MI-Blocks";
const char* pwd = "12345678";
const char* host = "10.42.0.1";
const int send_port = 5005;

void setup() {
    Serial.begin(115200);
    delay(2000);

    WiFi.disconnect(true, true);
    delay(1000);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pwd);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.print("WiFi connected, IP = ");
    Serial.println(WiFi.localIP());
}

void loop() {
    OscWiFi.update();
    
    float temp = temperatureRead();
    OscWiFi.send(host, send_port, "/xiao/c6_01/temperature", temp);
    Serial.println("Sent temp: " + String(temp));
    
    delay(500);
}