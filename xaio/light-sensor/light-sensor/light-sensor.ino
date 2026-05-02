#include <ArduinoOSCWiFi.h>

// ─── MODE FLAG ────────────────────────────────────────
#define MODE 'W'   // 'T' = test serial only, 'W' = wifi OSC

// ─── WIFI / OSC CONFIG ────────────────────────────────
const char* ssid      = "MI-Blocks";
const char* pwd       = "12345678";
const char* host      = "10.42.0.1";
const int   send_port = 5005;

// ─── HARDWARE CONFIG ──────────────────────────────────
#define LIGHT A0

// ─── SETUP ────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    delay(2000);

    #if MODE == 'W'
        WiFi.disconnect(true, true);
        delay(1000);
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, pwd);
        while (WiFi.status() != WL_CONNECTED) {
            Serial.print(".");
            delay(500);
        }
        Serial.println("\nConnected: " + WiFi.localIP().toString());
    #else
        Serial.println("TEST MODE — photoresistor only");
    #endif
}

// ─── LOOP ─────────────────────────────────────────────
void loop() {
    #if MODE == 'W'
        OscWiFi.update();
    #endif

    float lightVal = analogRead(LIGHT) / 4095.0;
    Serial.print("Light: "); Serial.println(lightVal);

    #if MODE == 'W'
        OscWiFi.send(host, send_port, "/xiao/c3_01/light", lightVal);
    #endif

    delay(20);
}