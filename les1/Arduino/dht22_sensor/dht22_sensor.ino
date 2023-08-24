#include "DHT.h"

#define DHT_PIN 7  // Pin waar de DHT sensor is aangesloten
#define DHT_TYPE DHT22  // Type van de DHT sensor (DHT11, DHT22, of AM2302)

const int LM35_PIN_3;

DHT dht(DHT_PIN, DHT_TYPE);  // Maak een DHT object aan

void setup() {
  Serial.begin(9600);  // Zet de seriele monitor aan
  dht.begin();  // Initialiseer de DHT sensor
}

void loop() {
  float humidity = dht.readHumidity();  // Lees de vochtigheid uit
  float temperature = dht.readTemperature();  // Lees de temperatuur uit

  // Controleer of de uitlezingen niet nul zijn en of de sensor niet faalt
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%  Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
  }

  delay(2000);  // Wacht 2 seconden voor de volgende uitlezing
}
