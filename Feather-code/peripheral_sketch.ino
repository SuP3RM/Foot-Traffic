#include <bluefruit.h>

BLEUart bleuart;

void setup() {
  Serial.begin(115200);
  
  Bluefruit.begin();
  Bluefruit.setName("Feather1");
  
  bleuart.begin();

  Bluefruit.Advertising.addService(bleuart);
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.start();
}

void loop() {
  bleuart.print("Hello from Feather 1");
  delay(1000);
}
