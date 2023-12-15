#include <bluefruit.h>

void setup() {
  Serial.begin(115200);
  // Initialize Bluefruit with 1 concurrent connection
  Bluefruit.begin(1, 0); 
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values

  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Start advertising
  Bluefruit.Advertising.start(0); // 0 = Don't stop advertising after n seconds
}

void loop() {
  // No need to do anything in the loop
}
