#include <bluefruit.h>

BLEUart bleuart;
BLEClientUart clientUart;

void setup() {
  Serial.begin(115200);
  
  Bluefruit.begin();
  Bluefruit.setName("Feather2");
  Bluefruit.Central.setDisconnectCallback(cent_disconnect_callback);
  Bluefruit.Central.setConnectCallback(cent_connect_callback);

  clientUart.begin();
  clientUart.setRxCallback(cent_uart_rx_callback);

  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.start(0);
}

void loop() {
  // Keep the loop empty
}

void scan_callback(ble_gap_evt_adv_report_t* report) {
  Bluefruit.Central.connect(report);
}

void cent_connect_callback(uint16_t conn_handle) {
  clientUart.discover(conn_handle);
}

void cent_uart_rx_callback(BLEClientUart& uart_svc) {
  while (uart_svc.available()) {
    Serial.print((char)uart_svc.read());
  }
}

void cent_disconnect_callback(uint16_t conn_handle, uint8_t reason) {
  (void) conn_handle;
  (void) reason;
  Bluefruit.Scanner.start(0);  // Start scanning again
}
