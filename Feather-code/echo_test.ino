
#include <Wire.h>
#include <Adafruit_USBD_CDC.h>
void setup() {
  Serial.begin(115200);
  Serial.println("Serial test started");
}

void loop() {
  Serial.println("Hello world");
  delay(1000);
}
