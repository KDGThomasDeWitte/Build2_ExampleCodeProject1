#include <BLEDevice.h>
#include <BLEServer.h>

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("Device connected...");
    };

    void onDisconnect(BLEServer* pServer) {
      Serial.println("Device disconnected...");
    }
};

void setup() {
  Serial.begin(9600);

  // Create the BLE Device
  BLEDevice::init("ESP32 - Connection example ");

  // Create the BLE Server
  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting for a client connection...");
}

void loop() {
}
