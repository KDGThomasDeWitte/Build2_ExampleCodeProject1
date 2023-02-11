#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
int txValue = 0;

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

#define DEVICE_INFORMATION_SERVICE "180A"
//#define BLE_UUID_HEART_RATE_SERVICE               "180D"
//#define BLE_UUID_HEART_RATE_MEASURMENT            "2A37"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Device connected...");
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("Device disconnected...");
    }
};

class MyReceivingCharacteristicCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
      Serial.println("=====START RECEIVE=====");
      Serial.print("Received value: ");

      for (int i = 0; i < rxValue.length(); i++) {
        Serial.print(rxValue[i]);
      }
      Serial.println();

      // Turn on/off the internal LED based on the command received from the app
      if (rxValue.find("1") != -1) {
        Serial.println("Turning ON!");
        digitalWrite(LED_BUILTIN, HIGH);
      } else if (rxValue.find("0") != -1) {
        Serial.println("Turning OFF!");
        digitalWrite(LED_BUILTIN, LOW);
      }

      Serial.println();
      Serial.println("=====END RECEIVE=====");
    }
  }
};

void setup() {
  Serial.begin(9600);

  // Set pinmode for the builtin LED
  pinMode(LED_BUILTIN, OUTPUT);

  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(DEVICE_INFORMATION_SERVICE);

  // Create a BLE Characteristic for notifying
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Create a BLE Characteristic for receiving values
  BLECharacteristic *pCharacteristicRx = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_RX,
                      BLECharacteristic::PROPERTY_WRITE
                    );
  pCharacteristicRx->setCallbacks(new MyReceivingCharacteristicCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
  // notify changed value
  if (deviceConnected) {
    txValue = random(-10, 20);

    // Conversion of txValue
    char txString[8];
    dtostrf(txValue, 1, 2, txString);

    // Setting the value to the characteristic
    pCharacteristic->setValue(txString);

    // Notifying the connected client
    pCharacteristic->notify();
    Serial.println("Sent value: " + String(txString));

    delay(500);
  }
}
