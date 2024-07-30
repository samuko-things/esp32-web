#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
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

AsyncWebServer server(80);

// put function declarations here:
void notFound(AsyncWebServerRequest *request);

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

  //-------- server endpoints ---------//
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", WEB_PAGE); });

  // Client sends a GET request to <IP>/ledstate?state=<val>
  server.on("/ledstate", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (request->hasParam("state")) {
                String state;
                state = request->getParam("state")->value();
                ledState = state.toInt();
              }
        request->send(200, "text/plain", String(ledState)); });

  // // Send a POST request to <IP>/post with a form field message set to <message>
  // server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request)
  //           {
  //       String message;
  //       message = request->getParam("message", true)->value();
  //       request->send(200, "text/plain", "Hello, POST: " + message); });

  server.on("/sensordata", HTTP_GET, [](AsyncWebServerRequest *request)
            {
        request->send(200, "text/plain", sensorDataJsonString); });

  server.onNotFound(notFound);
  server.begin();
  //------------------------------------//

  Serial.println("Server has started");
  sensorUpdateTime = millis();
}

void loop()
{
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

// put function definitions here:
void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}