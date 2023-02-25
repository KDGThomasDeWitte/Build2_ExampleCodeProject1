// Include required libraries
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Define constants
#define DHTPIN D2      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22  // DHT 22 (AM2302)
#define RELAYPIN D3    // Relay pin
#define SOIL_CAP A2    // Soil digital sensor pin
#define SOIL_ANALOG A1 // Soil analog sensor pin

// Insert your network credentials
#define WIFI_SSID "insert here"
#define WIFI_PASSWORD "insert here"

// Insert Firebase project API Key
#define API_KEY "insert here"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "insert here"

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Declare variables
unsigned long startpoint = 0;
boolean millis_check;
bool signupOK = false;
// Create DHT object
DHT_Unified dht(DHTPIN, DHTTYPE);

// Function to read temperature from DHT sensor
void get_temp()
{
  float temp;
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    temp = event.temperature;
  }
  else
  {
    temp = event.temperature;
  }
  Firebase.RTDB.setFloat(&fbdo, "Tempature/temp", temp);
}

// Function to checks soil wetness and returns a True when dry
void get_soilmoissture_check()
{
  /*
  https://wiki.dfrobot.com/Moisture_Sensor__SKU_SEN0114_
  Resistance sensor values:
    Dry: 0  ~300
    Wet: 300~700
    Water: 700~950
*/
  /*
   https://wiki.dfrobot.com/Capacitive_Soil_Moisture_Sensor_SKU_SEN0193
   Capacitive sensor values
      Dry: (520 430]
      Wet: (430 350]
      Water: (350 260]
  */
  Serial.println("Checking Soil...");
  int is_soil_dry;
  int resSensorValue = analogRead(SOIL_ANALOG);
  int capSensorValue = analogRead(SOIL_CAP);
  Serial.print("Resistor Soil Value:");
  Serial.println(resSensorValue);
  Serial.print("Resistor Cap Value:");
  Serial.println(capSensorValue);
  if (resSensorValue < 300 && capSensorValue > 430)
  {
    is_soil_dry = 1;
  }
  else
  {
    is_soil_dry = 0;
  }
  Firebase.RTDB.setInt(&fbdo, "Soil/status", is_soil_dry);
}

// Function to set relay state based on soil moisture status
void set_soil_relay()
{
// Get soil moisture status from Firebase Realtime Database
Firebase.RTDB.getInt(&fbdo, "Soil/status");
int turn_relay_on = fbdo.intData();
digitalWrite(RELAYPIN, turn_relay_on);
}

void setup()
{
  delay(2222);
  // Initialize serial communication
  Serial.begin(115200);
  // Initialize DHT sensor
  dht.begin();
  // Define relay pin as output
  pinMode(RELAYPIN, OUTPUT);
  // Initialize Dabble app
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("ok");
    signupOK = true;
  }
  else
  {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop()
{
  set_soil_relay();
  // Check if 10 seconds have elapsed since the last data transmission
  if (startpoint + 10000 < millis())
  {
    startpoint = millis();
    millis_check = true;
  }
  // If 10 seconds have elapsed
  if (millis_check)
  {
    millis_check = false;
    if (Firebase.ready() && signupOK)
    {
      get_soilmoissture_check();
      get_temp();
    }
  }
}
