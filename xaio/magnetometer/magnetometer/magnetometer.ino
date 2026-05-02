#include <ArduinoOSCWiFi.h>
#include <QMC5883LCompass.h>

// ─── MODE FLAG ────────────────────────────────────────
#define MODE 'T'   // 'T' = test serial only, 'W' = wifi OSC

// ─── WIFI / OSC CONFIG ────────────────────────────────
const char* ssid      = "MI-Blocks";
const char* pwd       = "12345678";
const char* host      = "10.42.0.1";
const int   send_port = 5005;

// ─── HARDWARE CONFIG ──────────────────────────────────
QMC5883LCompass compass;

// ─── SETUP ────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    delay(2000);

    compass.init();

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
        Serial.println("TEST MODE — magnetometer only");
    #endif
}

// ─── LOOP ─────────────────────────────────────────────
void loop() {
    #if MODE == 'W'
        OscWiFi.update();
    #endif

    compass.read();
    int x = compass.getX();
    int y = compass.getY();
    int z = compass.getZ();

    Serial.print(x); Serial.print(" ");
    Serial.print(y); Serial.print(" ");
    Serial.println(z);

    #if MODE == 'W'
        OscWiFi.send(host, send_port, "/xiao/c3_01/compass_x", (float)x);
        OscWiFi.send(host, send_port, "/xiao/c3_01/compass_y", (float)y);
        OscWiFi.send(host, send_port, "/xiao/c3_01/compass_z", (float)z);
    #endif

    delay(100);
}