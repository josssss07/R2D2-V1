#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include <L298NX2.h>

// Wi-Fi connectivity config
const char *ssid = "";
const char *password = "";
AsyncWebServer server(80);      // Create AsyncWebServer object on port 80
AsyncWebSocket ws("/ws");       // Create a WebSocket object
JSONVar readings;               // Json Variable to Hold Sensor Readings

// Motor config
const unsigned int EN_A = 1;
const unsigned int IN1_A = 22;
const unsigned int IN2_A = 23;

const unsigned int IN1_B = 19;
const unsigned int IN2_B = 21;
const unsigned int EN_B = 3;
const unsigned int START_SPEED = 60;

L298NX2 motors(EN_A, IN1_A, IN2_A, EN_B, IN1_B, IN2_B);

void messageReceived(void *, uint8_t *, size_t);

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
    return;
  }
  Serial.println("SPIFFS mounted.");
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
      Serial.println("An error occured while a client tried to connect.");
      break;
  }
}

void notifyClients(String reply) {
  ws.textAll(reply);
}

String handleMessage(uint8_t *data){
  char *msg = (char*)data;

  if(strcmp(msg, "s") == 0){ 
    motors.stop();
    return "Stop";
  }
  else if(strcmp(msg, "ts") == 0){ 
    motors.stop();
     return "Stopped";
  } 
  else if(strcmp(msg, "read") == 0){ 
     return "Speed: " + motors.getSpeedA();
  } 
  else if (strcmp(msg, "f") == 0) {
     motors.forward();
    return "Moving Forward";
  } 
  else if(strcmp(msg, "b") == 0) {
     motors.backward();
    return "Moving Behind";
  } 
  else if(strcmp(msg, "l") == 0) {
     motors.forwardA();
     motors.backwardB();
    return "Moving Left";
  } 
  else if(strcmp(msg, "r") == 0) {
     motors.backwardA();
     motors.forwardB();
    return "Moving Right";
  }
   else if(strcmp(msg, "spup") == 0){ 
     unsigned int upped = motors.getSpeedA() - 15;
     upped = upped > 0 ? upped : 0;
     motors.setSpeed(upped);
    return "Speed upped by 15";
  } 

  else if(strcmp(msg, "spdw") == 0){ 
     unsigned int downed = motors.getSpeedA() + 15;
     downed = downed < 230 ? downed : 230;
     motors.setSpeed(downed);
    return "Speed downed by 15";
  } 
}

void messageReceived(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;

  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    String message = (char*)data;

    //Check if the message is "getReadings"
    String reply = handleMessage(data);
    Serial.print(reply);
    notifyClients(reply);
  }
}

void setup() {
  Serial.begin(115200);
  initSPIFFS();
  initCommsSystems();
  // motors.setSpeed(START_SPEED);
  // motors.forward();
}

void loop() {
  ws.cleanupClients();
}
