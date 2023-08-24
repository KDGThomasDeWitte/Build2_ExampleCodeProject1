const int SEN0114_PIN = A1;
const int SEN0193_PIN = A2;
const int RELAY = 3;

const int RESISTANCE_MIN = 0;
const int RESISTANCE_MAX = 760;
const int CAPACITANCE_MAX = 250;
const int CAPACITANCE_MIN = 516;

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

void setup() {
  Serial.begin(9600);

  pinMode(RELAY, OUTPUT);
}  

void loop() {
  int resSensorValue = analogRead(SEN0114_PIN);
  int resMoisturePercentage = calculateMoisturePercentage(resSensorValue, RESISTANCE_MIN, RESISTANCE_MAX); 

  int capSensorValue = analogRead(SEN0193_PIN);
  int capMoisturePercentage = calculateMoisturePercentage(capSensorValue, CAPACITANCE_MIN, CAPACITANCE_MAX);
  
  outputSerialSoilValues("Resistance type ", resSensorValue, resMoisturePercentage);
  outputSerialSoilValues("Capacitance type ", capSensorValue, capMoisturePercentage);
  delay(1000);

  digitalWrite(RELAY, HIGH);   //Turn on relay
  delay(2000);
  digitalWrite(RELAY, LOW);    //Turn off relay
  delay(2000);
}

int calculateMoisturePercentage(int sensorValue, int sensorMinValue, int sensorMaxValue) {
    return map(sensorValue, sensorMaxValue, sensorMinValue, 100, 0);
}

void outputSerialSoilValues(String soilSensorType, int rawValue, int percentage) {
  Serial.print(soilSensorType + "soil sensor value: ");
  Serial.print(rawValue);
  Serial.print(" - Soil moisture percentage: ");
  Serial.print(percentage);
  Serial.println("%");
}