// Include required libraries
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <DabbleESP32.h>

// Define constants
#define DHTPIN D2 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT 22 (AM2302)
#define RELAYPIN D3 // Relay pin

// Define required settings for Dabble app
#define CUSTOM_SETTINGS
#define INCLUDE_DATALOGGER_MODULE
#define INCLUDE_DABBLEINPUTS_MODULE

// Declare variables
unsigned long startpoint = 0;
boolean millis_check;
// Create DHT object
DHT_Unified dht(DHTPIN, DHTTYPE);

// Function to read temperature from DHT sensor
float get_temp(){float temp;
  startpoint = millis();
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    temp = event.temperature;
  }
  else {
    temp = event.temperature;
  }
    Serial.print("Temp:");
    Serial.println(temp);
  return temp;
}

// Function to set the status of the relay based on Dabble input
void set_relay(){
  Dabble.processInput();
  digitalWrite(RELAYPIN, Inputs.getTactileSwitch1Value());
  Serial.print("Relay status: ");
  Serial.println(Inputs.getTactileSwitch1Value());
}

// Function to initialize the data logger
void initializeFile()
{
  DataLogger.createFile("TEMP DATA"); // Create data file (mandatory step)
  DataLogger.createColumn("TEMP"); // Add column names (mandatory step)
}

// Function to send temperature data to GSM module
void send_to_gsm(){
  Dabble.processInput();
  DataLogger.send("TEMP",get_temp());
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  // Initialize DHT sensor
  dht.begin();
  // Define relay pin as output
  pinMode(RELAYPIN, OUTPUT);
  // Initialize Dabble app
  Dabble.begin("ESP_THOMAS");
  // Send data logger settings to Dabble app
  DataLogger.sendSettings(&initializeFile);
  // Call initializeFile function
  initializeFile();
}

void loop() {
  set_relay();
  // Check if 2 seconds have elapsed since the last data transmission
  if (startpoint + 2000 < millis()){
  startpoint = millis();
  millis_check = true;
  }
  // If 2 seconds have elapsed, send temperature data to GSM module and reset flag
  if (millis_check){
  send_to_gsm();
  millis_check = false;
  }
}
