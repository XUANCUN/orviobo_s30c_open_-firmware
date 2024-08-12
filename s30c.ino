#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "FS.h"
#include <OneButton.h>


ESP8266WebServer server(80);

//const int LED_PIN = LED_BUILTIN;
const int LED_PIN = 5;
int ledStatus = HIGH;
OneButton button(14,LOW);

void handleRoot() {
    // Just serve the index page from SPIFFS when asked for
    File index = SPIFFS.open("/index.html", "r");
    server.streamFile(index, "text/html");
    index.close();
}


// A function which sends the LED status back to the client
void sendStatus() {
    if (ledStatus == LOW) server.send(200, "text/plain", "HIGH");
    else server.send(200, "text/plain", "LOW");
}
void onClick() {
  toggleLED();
}
// Toggle the LED and back its status
void toggleLED() {
    ledStatus = ledStatus == HIGH ? LOW : HIGH;
    digitalWrite(LED_PIN, ledStatus);
    sendStatus();
}


void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, ledStatus);
    Serial.begin(115200);
    
    button.attachClick(onClick);
    SPIFFS.begin();
    WiFiManager wm;
    wm.setConfigPortalTimeout(180);
    wm.autoConnect("SmartSocket");


    Serial.println(""); 
    Serial.print("ESP8266 Connected to ");
    Serial.println(WiFi.SSID());              // WiFi名称
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());  

 
    // The root handler
    server.on("/", handleRoot);
    // Handlers for various user-triggered events
    server.on("/toggle", toggleLED);
    server.on("/status", sendStatus);
    

    server.begin();
    
}

void loop() {
    server.handleClient();
    button.tick();
    
    
}
