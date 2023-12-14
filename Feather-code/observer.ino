#include <bluefruit.h>

// Define an RSSI threshold for detecting foot traffic
const int RSSI_THRESHOLD = -70; // Adjust this value based on your testing

void scan_callback(ble_gap_evt_adv_report_t* report) {
  int rssi = report->rssi;
  Serial.print("RSSI: ");
  Serial.println(rssi);

  // Check if the RSSI value indicates potential foot traffic
  if (rssi < RSSI_THRESHOLD) {
    Serial.println("Foot traffic detected");
    // Record the event or take necessary actions
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize Bluefruit with 0 concurrent connections as peripheral and 1 as central
  Bluefruit.begin(0, 1);
  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.start(0); // 0 = continuous scanning
}

void loop() {
  // No need to do anything in the loop
}
