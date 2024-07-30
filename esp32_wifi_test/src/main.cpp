#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// const char *HOTSPOT_SSID = "esp32_hotspot";
// const char *HOTSPOT_PASSWORD = "password";

// IPAddress local_ip(192, 168, 1, 1);
// IPAddress gateway(192, 168, 1, 1);
// IPAddress subnet(255, 255, 255, 0);

const char *WIFI_SSID = "sammyspot";
const char *WIFI_PASSWORD = "mobo_bot";

bool ledStatus = LOW;

WebServer server(80); // http is on port 80

//-------------------------------------------------//
// put function declarations here:
String getHTML();
void handle_OnConnect();
void handle_OffLed();
void handle_OnLed();
void handle_NotFound();
//-------------------------------------------------//

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  ledStatus = LOW;

  // WiFi.softAP(HOTSPOT_SSID, HOTSPOT_PASSWORD);
  // WiFi.softAPConfig(local_ip, gateway, subnet);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, HIGH);
  bool ledStatus = HIGH;

  //-------- server endpoints ---------//
  server.on("/", handle_OnConnect);
  server.on("/off", handle_OffLed);
  server.on("/on", handle_OnLed);
  server.onNotFound(handle_NotFound);
  server.begin();
  //------------------------------------//

  Serial.println("Server has started");
}

void loop()
{
  server.handleClient();

  if (ledStatus)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
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
String getHTML()
{
  String htmlCode = R"(
  <!DOCTYPE html>
  <html lang="en">
    <head>
      <meta charset="UTF-8" />
      <meta name="viewport" content="width=device-width, initial-scale=1.0" />
      <title>ESP32 LED CONTROL</title>
    </head>
    <body>
      <h1>ESP32 WEB SERVER</h1>
      <h3>Simple Led Control</h3>
      <p>LED STATUS: <b>{val1}</b></p>
      <p>{val2}</p>
    </body>
  </html>
  )";

  if (ledStatus)
  {
    htmlCode.replace("{val1}", "ON");
    htmlCode.replace("{val2}", R"(<a href="/off">Turn Off</a>)");
  }
  else
  {
    htmlCode.replace("{val1}", "OFF");
    htmlCode.replace("{val2}", R"(<a href="/on">Turn On</a>)");
  }

  return htmlCode;
}

void handle_OnConnect()
{
  ledStatus = LOW;
  Serial.println("LED STATUS: OFF");
  server.send(200, "text/html", getHTML());
}

void handle_OffLed()
{
  ledStatus = LOW;
  Serial.println("LED STATUS: OFF");
  server.send(200, "text/html", getHTML());
}

void handle_OnLed()
{
  ledStatus = HIGH;
  Serial.println("LED STATUS: ON");
  server.send(200, "text/html", getHTML());
}

void handle_NotFound()
{
  Serial.println("Route Not Found");
  server.send(404, "text/plain", "Route Not Found");
}
//-------------------------------------------------//