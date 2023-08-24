const int TMP36_PIN = A3;
const int TMP36_TEMP_OFFSET = 500;

void setup() {
  Serial.begin(9600);
}  

void loop() {
  int sensorValue = analogRead(TMP36_PIN);
  float voltage = rawSensorValueToVoltage(sensorValue);
  float temperature = voltageToTemperature(voltage);
  
  outputSerialValues(sensorValue, voltage, temperature);
  delay(200);
}

float rawSensorValueToVoltage(int rawSensorValue) {
  return (rawSensorValue / 1023.0) * 5000; // 5000 voor milli Volts
}

float voltageToTemperature(float voltage) {
  return (voltage - TMP36_TEMP_OFFSET) * 0.1;
}

void outputSerialValues(int rawValue, float voltage, float temperature) {
  Serial.print("Sensor value: ");
  Serial.print(rawValue);
  Serial.print(" - Voltage: ");
  Serial.print(voltage);
  Serial.print(" - Temperature: ");
  Serial.print(temperature);
  Serial.println(" degrees (Celcius)");
}