#include <ArduinoOSCWiFi.h>

// ─── MODE FLAG ────────────────────────────────────────
#define MODE 'W'   // 'T' = test serial only, 'W' = wifi OSC

// ─── WIFI / OSC CONFIG ────────────────────────────────
const char* ssid      = "MI-Blocks";
const char* pwd       = "12345678";
const char* host      = "10.42.0.1";
const int   send_port = 5005;

// ─── HARDWARE CONFIG ──────────────────────────────────
#define SLIDER    A0
#define BTN_PLAY  D2
#define BTN_PAUSE D4

bool lastPlayState  = HIGH;
bool lastPauseState = HIGH;
unsigned long lastPlayTime  = 0;
unsigned long lastPauseTime = 0;
const int BTN_DEBOUNCE = 50;

// ─── SETUP ────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    delay(2000);

    pinMode(BTN_PLAY,  INPUT_PULLUP);
    pinMode(BTN_PAUSE, INPUT_PULLUP);

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
        Serial.println("TEST MODE — slider + play/pause");
    #endif
}

// ─── LOOP ─────────────────────────────────────────────
void loop() {
    #if MODE == 'W'
        OscWiFi.update();
    #endif

    unsigned long now = millis();

    // slider
    float sliderVal = analogRead(SLIDER) / 4095.0;
    Serial.print("Slider: "); Serial.println(sliderVal);
    #if MODE == 'W'
        OscWiFi.send(host, send_port, "/xiao/c3_01/slider", sliderVal);
    #endif

    // play button
    bool currentPlay = digitalRead(BTN_PLAY);
    if (currentPlay == LOW && lastPlayState == HIGH && (now - lastPlayTime) > BTN_DEBOUNCE) {
        Serial.println("PLAY");
        #if MODE == 'W'
            OscWiFi.send(host, send_port, "/xiao/c3_01/play", 1.0f);
        #endif
        lastPlayTime = now;
    }
    lastPlayState = currentPlay;

    // pause button
    bool currentPause = digitalRead(BTN_PAUSE);
    if (currentPause == LOW && lastPauseState == HIGH && (now - lastPauseTime) > BTN_DEBOUNCE) {
        Serial.println("PAUSE");
        #if MODE == 'W'
            OscWiFi.send(host, send_port, "/xiao/c3_01/pause", 1.0f);
        #endif
        lastPauseTime = now;
    }
    lastPauseState = currentPause;

    delay(20);
}