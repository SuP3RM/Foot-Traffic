#include <bluefruit.h>

BLEDis bledis;  // Device Information Service
BLEUart bleuart;  // BLE UART service

void setup() {
  Serial.begin(115200);

  // Initialize Bluefruit with 1 peripheral and 0 central
  Bluefruit.begin(1, 0);
  Bluefruit.setName("Feather Peripheral");

  // Set up and start advertising
  bledis.setManufacturer("Adafruit");
  bledis.setModel("Feather nRF52840");
  bledis.begin();

  // Configure and start BLE UART service
  bleuart.begin();

  // Start advertising
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addService(bleuart);
  Bluefruit.Advertising.start(0); // 0 = Don't stop advertising after n seconds
}

void loop() {
  if (bleuart.available()) {
    String msg = bleuart.readString();
    Serial.println("Received: " + msg);
  }
}
