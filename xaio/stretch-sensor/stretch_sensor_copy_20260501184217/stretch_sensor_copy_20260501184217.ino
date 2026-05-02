#include <ArduinoOSCWiFi.h>

// ─── MODE FLAG ────────────────────────────────────────
#define MODE 'W'   // 'T' = test serial only, 'W' = wifi OSC

// ─── WIFI / OSC CONFIG ────────────────────────────────
const char* ssid      = "MI-Blocks";
const char* pwd       = "12345678";
const char* host      = "10.42.0.1";
const int   send_port = 5005;

// ─── HARDWARE CONFIG ──────────────────────────────────
#define STRETCH A2

const float RAW_MIN = 0.70;
const float RAW_MAX = 0.80;

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
        Serial.println("TEST MODE — stretch sensor only");
    #endif
}

// ─── LOOP ─────────────────────────────────────────────
void loop() {
    #if MODE == 'W'
        OscWiFi.update();
    #endif

    float raw    = analogRead(STRETCH) / 4095.0;
    float mapped = constrain((raw - RAW_MIN) / (RAW_MAX - RAW_MIN), 0.0, 1.0);

    Serial.print("Raw: "); Serial.print(raw);
    Serial.print("  Mapped: "); Serial.println(mapped);

    #if MODE == 'W'
        OscWiFi.send(host, send_port, "/xiao/c3_01/stretch", mapped);
    #endif

    delay(50);
}