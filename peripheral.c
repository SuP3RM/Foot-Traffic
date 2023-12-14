#include <bluefruit.h>
void setup() {
  Serial.begin(115200);
  while (!Serial);
  Bluefruit.begin(1, 0);
  Bluefruit.setTxPower(4);
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addName(); // Include the device name in advertising
  Bluefruit.Advertising.setInterval(20, 40); // Even faster advertising
  Bluefruit.Advertising.start(0);
  Serial.println("Started BLE advertising...");
}
void loop() {
  static unsigned long counter = 0; // Counter for debugging
  Serial.print("Advertising #");
  Serial.println(counter++);
  delay(1000);
  // Manually restart advertising periodically
  if (counter % 60 == 0) { // Restart advertising every 60 seconds
    Bluefruit.Advertising.stop();
    Bluefruit.Advertising.start(0);
  }
}
