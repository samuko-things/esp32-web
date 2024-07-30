#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include "webpage.h"

// const char *HOTSPOT_SSID = "esp32_hotspot";
// const char *HOTSPOT_PASSWORD = "password";

// IPAddress local_ip(192, 168, 1, 1);
// IPAddress gateway(192, 168, 1, 1);
// IPAddress subnet(255, 255, 255, 0);

const char *WIFI_SSID = "sammyspot";
const char *WIFI_PASSWORD = "mobo_bot";

int ledState = 0;
float sensorA, sensorB, sensorC;
unsigned long sensorUpdateTime;

String sensorDataJsonString;
JsonDocument sensorDataJsonDoc;

JsonDocument doc;
String docString;
String action;

WebServer webServer(80);                                 // http is on port 80
WebSocketsServer webSocketServer = WebSocketsServer(81); // the websocket uses port 81 (standard port for websockets )

//-------------------------------------------------//
// put function declarations here:
void handle_OnConnect();
void handle_NotFound();
void webSocketEvent(byte clientNum, WStype_t type, uint8_t *payload, size_t length);
//-------------------------------------------------//

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // WiFi.softAP(HOTSPOT_SSID, HOTSPOT_PASSWORD);
  // WiFi.softAPConfig(local_ip, gateway, subnet);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(500);
    Serial.print(".");
  }

  ledState = 0;
  digitalWrite(LED_BUILTIN, ledState);

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //-------- initialize webServer ---------//
  webServer.on("/", handle_OnConnect);
  webServer.onNotFound(handle_NotFound);
  webServer.begin();
  //--------------------------------------//

  //------- initialize webSocketServer ---------//
  webSocketServer.begin(); // start websocket
  webSocketServer.onEvent(webSocketEvent);
  //--------------------------------------------//

  Serial.println("Server has started");
  sensorUpdateTime = millis();
}

void loop()
{
  webServer.handleClient();
  webSocketServer.loop();
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, ledState);

  if ((millis() - sensorUpdateTime) >= 50)
  {
    // Serial.println("Reading Sensors");
    sensorA = (float)random(100, 1000) / 100.0;
    sensorB = (float)random(100, 1000) / 100.0;
    sensorC = (float)random(100, 1000) / 100.0;

    sensorDataJsonDoc["sensorA"] = sensorA;
    sensorDataJsonDoc["sensorB"] = sensorB;
    sensorDataJsonDoc["sensorC"] = sensorC;
    serializeJson(sensorDataJsonDoc, sensorDataJsonString); // convert JSON object to string
    // serializeJsonPretty(sensorDataJsonDoc, Serial);

    sensorUpdateTime = millis();
  }

  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(500);
    Serial.print(".");
  }
}

//-------------------------------------------------//
// put function definitions here:

void handle_OnConnect()
{
  webServer.send(200, "text/html", WEB_PAGE);
  Serial.println("Home Page Loaded");
}

void handle_NotFound()
{
  webServer.send(404, "text/plain", "Route Not Found");
  Serial.println("Route Not Found");
}

void webSocketEvent(byte clientNum, WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.println("Client " + String(clientNum) + " disconnected");
    break;
  case WStype_CONNECTED:
    Serial.println("Client " + String(clientNum) + " connected");
    break;
  case WStype_TEXT:
    if (strcmp((char *)payload, "toggleLED") == 0)
    {
      ledState = (ledState == 0) ? 1 : 0;
      doc.clear();
      doc["action"] = "getLedState";
      doc["value"] = ledState;
      serializeJson(doc, docString);
      webSocketServer.sendTXT(clientNum, docString);
    }
    else if (strcmp((char *)payload, "getLedState") == 0)
    {
      doc.clear();
      doc["action"] = "getLedState";
      doc["value"] = ledState;
      serializeJson(doc, docString);
      webSocketServer.sendTXT(clientNum, docString);
    }
    else if (strcmp((char *)payload, "getSensorData") == 0)
    {
      doc.clear();
      doc["action"] = "getSensorData";
      doc["value"] = sensorDataJsonDoc;
      serializeJson(doc, docString);
      webSocketServer.sendTXT(clientNum, docString);
    }

  // For everything else: do nothing
  // case WStype_BIN:
  // case WStype_ERROR:
  // case WStype_FRAGMENT_TEXT_START:
  // case WStype_FRAGMENT_BIN_START:
  // case WStype_FRAGMENT:
  // case WStype_FRAGMENT_FIN:
  default:
    break;
  }
}
//-------------------------------------------------//

// void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
// {

//   switch (type)
//   {
//   case WStype_DISCONNECTED:
//     Serial.printf("[%u] Disconnected!\n", num);
//     break;

//   case WStype_CONNECTED:
//   {
//     IPAddress ip = webSocket.remoteIP(num);
//     Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
//     // send message to client
//     webSocket.sendTXT(num, "0");
//   }
//   break;

//   case WStype_TEXT:
//     Serial.printf("[%u] get Text: %s\n", num, payload);
//     // send message to client
//     // webSocket.sendTXT(num, "message here");
//     // send data to all connected clients
//     // webSocket.broadcastTXT("message here");
//     break;

//   case WStype_BIN:
//     Serial.printf("[%u] get binary length: %u\n", num, length);
//     hexdump(payload, length);
//     // send message to client
//     // webSocket.sendBIN(num, payload, length);
//     break;
//   }
// }