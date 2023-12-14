#include <bluefruit.h>
// Threshold for significant RSSI change
const int RSSI_FLUCTUATION_THRESHOLD = -70; // Adjust based on your environment
int lastRssi = 0;
unsigned long lastDetectionTime = 0;
const unsigned long detectionInterval = 2000; // 2 seconds to avoid multiple counts for same event
void scan_callback(ble_gap_evt_adv_report_t* report) {
  int rssi = report->rssi;
  unsigned long currentTime = millis();
  // Detect significant RSSI fluctuation
  if (lastRssi != 0 && abs(lastRssi - rssi) >= RSSI_FLUCTUATION_THRESHOLD) {
    if (currentTime - lastDetectionTime > detectionInterval) {
      Serial.println("Foot traffic detected");
      lastDetectionTime = currentTime;
    }
  }
  lastRssi = rssi;
  Serial.print("Detected BLE Device, RSSI: ");
  Serial.println(rssi);
}
void startScanning() {
  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.useActiveScan(true);
  Bluefruit.Scanner.setInterval(100, 50); // Interval and window in units of 0.625 ms
  Bluefruit.Scanner.start(0); // Start continuous scanning
}
void setup() {
  Serial.begin(115200);
  while (!Serial);
  Bluefruit.begin(0, 1);
  startScanning();
  Serial.println("Starting Foot Traffic Counter...");
}
void loop() {
  static unsigned long lastScanRestart = millis();
  // Restart scanning every second to refresh BLE scanning
  if (millis() - lastScanRestart > 1000) {
    Bluefruit.Scanner.stop();
    startScanning();
    lastScanRestart = millis();
    Serial.println("Restarted BLE Scanning...");
  }
}
