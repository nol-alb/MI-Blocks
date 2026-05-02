#include <ArduinoOSCWiFi.h>

// ─── MODE FLAG ────────────────────────────────────────
#define MODE 'W'   // 'T' = test serial only, 'W' = wifi OSC

// ─── WIFI / OSC CONFIG ────────────────────────────────
const char* ssid      = "MI-Blocks";
const char* pwd       = "12345678";
const char* host      = "10.42.0.1";
const int   send_port = 5005;

// ─── HARDWARE CONFIG ──────────────────────────────────
#define REC_BTN D1

bool recording        = false;
bool lastBtnState     = HIGH;
unsigned long lastBtnTime = 0;
const int BTN_DEBOUNCE  = 50;

// ─── SETUP ────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    delay(2000);

    pinMode(REC_BTN, INPUT_PULLUP);

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
        Serial.println("TEST MODE — record button only");
    #endif
}

// ─── LOOP ─────────────────────────────────────────────
void loop() {
    #if MODE == 'W'
        OscWiFi.update();
    #endif

    unsigned long now    = millis();
    bool currentBtnState = digitalRead(REC_BTN);

    // toggle recording on button press
    if (currentBtnState == LOW && lastBtnState == HIGH && (now - lastBtnTime) > BTN_DEBOUNCE) {
        recording = !recording;
        Serial.println(recording ? "RECORDING" : "STOPPED");

        #if MODE == 'W'
            OscWiFi.send(host, send_port, "/xiao/c3_01/record", recording ? 1.0f : 0.0f);
        #endif

        lastBtnTime = now;
    }

    lastBtnState = currentBtnState;
    delay(10);
}