#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>

const char *ssid = "Gilbert";
const char *password = "46643753";
AsyncWebServer server(80);      // Create AsyncWebServer object on port 80
AsyncWebSocket ws("/ws");       // Create a WebSocket object
JSONVar readings;               // Json Variable to Hold Sensor Readings

void initWiFi() {
  //Start up the onboard WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  
  Serial.println(WiFi.localIP());
}

void initWebSocket() {
  ws.onEvent(onEvent);          //Upon receiving any transmission on the WebSocket port, execute callback
  server.addHandler(&ws);       // Register the WebSocket object with the WebServer
}

void initCommsSystems(){
  initWiFi();
  initWebSocket();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {       // Serve the HTML controller file
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.serveStatic("/", SPIFFS, "/");

  server.begin();
}

void initSPIFFS() {
  //Mount the ESP filesystem
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *frame, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      messageReceived(frame, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      Serial.println("An error occured while a client tried to connect.")
      break;
  }
}

void setup() {
  initCommsSystems();
  initSPIFFS();

  Serial.begin(115200);
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
}

void loop() {
  ws.cleanupClients();
}
