#include <WiFi.h>
#include <HTTPClient.h>
#include <credentials.h>

int status = WL_IDLE_STATUS;     // the Wifi radio's status

void setup(){
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  // Print status
  Serial.print("Wifi status: ");
  Serial.println(WiFi.status());

  // Print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Print signal strength
  Serial.print("\tSignal: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");

  HTTPClient http;
  http.begin("https://api.open-meteo.com/v1/forecast?latitude=74.0324&longitude=40.7440&current_weather=true");
  int httpResponseCode = http.GET();
      
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  
  // Free resources
  http.end();

  // Disconnect
  WiFi.disconnect();
  Serial.println("Disconnecting ...");
 
  // Print status
  Serial.print("Wifi status: ");
  Serial.println(WiFi.status());
}

void loop() {
}
