#include <ArduinoOSCWiFi.h>

// ─── MODE FLAG ────────────────────────────────────────
#define MODE 'W'   // 'T' = test serial only, 'W' = wifi OSC

// ─── WIFI / OSC CONFIG ────────────────────────────────
const char* ssid      = "MI-Blocks";
const char* pwd       = "12345678";
const char* host      = "10.42.0.1";
const int   send_port = 5005;

// ─── HARDWARE CONFIG ──────────────────────────────────
#define TRIG D2
#define ECHO D3

// ─── SETUP ────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    delay(2000);

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

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
        Serial.println("TEST MODE — ultrasonic only");
    #endif
}

// ─── LOOP ─────────────────────────────────────────────
void loop() {
    #if MODE == 'W'
        OscWiFi.update();
    #endif

    // read ultrasonic
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    long duration = pulseIn(ECHO, HIGH);
    float distance = duration * 0.034 / 2;

    Serial.print("Distance: "); Serial.print(distance); Serial.println(" cm");

    #if MODE == 'W'
        OscWiFi.send(host, send_port, "/xiao/c6_01/ultrasonic", distance);
    #endif

    delay(100);
}