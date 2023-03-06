#include "WiFi.h"
#include "ESPAsyncWebServer.h"

const char* PARAM_PIN_NUMBER = "output";
String LED_BUILTIN_NUMBER = "2";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <title>Dashboard Plantbewatering</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
        html {
            font-family: Arial;
            display: inline-block;
            text-align: center;
        }
        h2 {
            font-size: 3.0rem;
        }
        p {
            font-size: 1.5rem;
        }
        body {
            max-width: 600px;
            margin: 0px auto;
            padding-bottom: 25px;
        }
    </style>
</head>
<body>
  <h2>Dashboard plantbewatering</h2>
  <p>
     Percentage bodemvochtigheid: %BODEMVOCHTIGHEID%
  </p>
  <form>
     %BUTTONPLACEHOLDER%
  </form>

<script>
  function clickButton(element) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/update?output=" + element.id, true);
    xhr.send();
  }
</script>
</body>
</html>
)rawliteral";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void setupWifiAccessPoint() {
  // Set Wifi mode to both standard connection and AP
  WiFi.mode(WIFI_AP_STA);

  // Make sure the password is at least 8 characters long
  Serial.println("Setting up soft access point...");
  WiFi.softAP("wifi_hans", "password");

  // Display IP address to use for webserver
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

String geefBodemvochtigheid() {
  // Zet hier code die het vochtigheidspercentage van je gecalibreerde sensor teruggeeft
  return "50";
}

// Replaces placeholders with buttons or value section in your web page
String replacePlaceholdersInHtml(const String& var){
  if (var == "BUTTONPLACEHOLDER") {
    String buttons = "<input type=\"button\" value=\"Plant bewateren\" onclick=\"clickButton(this)\" id=\"" + LED_BUILTIN_NUMBER + "\">";
    return buttons;
  } else if (var == "BODEMVOCHTIGHEID") {
    return geefBodemvochtigheid();
  }
  return String();
}

void bewaterPlant(int pinNumber) {
  // Zet hier code die je plant bewatert
  Serial.println("Plant bewateren...");
  digitalWrite(pinNumber, HIGH);
  // Don't use delay, just for demo purpose
  delay(2000); 
  digitalWrite(pinNumber, LOW);
}

void setup(){
  Serial.begin(9600);

  // Set pinmode for the builtin LED
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Setup Wifi access point
  setupWifiAccessPoint();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, replacePlaceholdersInHtml);
  });

  // Send a GET request to <ESP_IP>/update?output=<pinNumber>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->hasParam(PARAM_PIN_NUMBER)) {
      String pinNumber = request->getParam(PARAM_PIN_NUMBER)->value();
      Serial.print("Parameter pinNumber: ");
      Serial.println(pinNumber);
      bewaterPlant(pinNumber.toInt());
    } else {
      Serial.println("No pinNumber parameter found.");
    }
    
    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {
}
