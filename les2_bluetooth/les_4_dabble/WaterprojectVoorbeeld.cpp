#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <DabbleESP32.h>

#define DHTPIN D2     // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
#define RELAYPIN D3 // relay pin

#define CUSTOM_SETTINGS
#define INCLUDE_DATALOGGER_MODULE
#define INCLUDE_DABBLEINPUTS_MODULE

unsigned long startpoint = 0;
boolean millis_check;
// Create DHT object
DHT_Unified dht(DHTPIN, DHTTYPE);

float get_temp(){
  float temp;
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

void set_relay(){
  Dabble.processInput();
  digitalWrite(RELAYPIN, Inputs.getTactileSwitch1Value());
  Serial.print("Relay status: ");
  Serial.println(Inputs.getTactileSwitch1Value());
}

void initializeFile()
{
  DataLogger.createFile("TEMP DATA");    //create file  (This is a mandatory step)
  DataLogger.createColumn("TEMP");               //enter column names (This is a mandatory step)         
}

void send_to_gsm(){
  Dabble.processInput();
  DataLogger.send("TEMP",get_temp());
}


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  //Initialize DHT sensor
  dht.begin();
  //define relaypin
  pinMode(RELAYPIN, OUTPUT);
  //Initialize Dabble
  Dabble.begin("ESP_THOMAS");
  DataLogger.sendSettings(&initializeFile);
  initializeFile();
}

void loop() {
  set_relay();
  if (startpoint + 2000 < millis()){
    startpoint = millis();
    millis_check = true;
  }
  if (millis_check){
    send_to_gsm();
    millis_check = false;
  }
}
