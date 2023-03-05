#include "WiFi.h"
#include "HTTPClient.h"
#include "ESPAsyncWebServer.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void setup(){
  Serial.begin(9600);

  WiFi.softAP("wifi_hans", "password");
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "temp");
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "hum");
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "press");
  });

  server.begin();
}

void loop() {
}
