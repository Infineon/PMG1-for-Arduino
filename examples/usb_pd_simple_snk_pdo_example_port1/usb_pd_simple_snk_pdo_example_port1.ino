// usb_pd_simple_snk_pdo_example_port1.ino
// This example code is available for PMGDuino only
// This example code demonstrates the functionalities of
// (1) How to initiate the USB-C port 1
// (2) How to add new sink PDOs in run-time
void setup() {
  // Initiate USB-C port 0
  usbpd1.begin();
  // Add 9V/3A, 12V/3A, 15V/3A and 20V/3A fixed PDO
  usbpd1.addFixedSnkPdo(1, 9000, 3000);
  usbpd1.addFixedSnkPdo(2, 12000, 3000);
  usbpd1.addFixedSnkPdo(3, 15000, 3000);
  usbpd1.addFixedSnkPdo(4, 20000, 3000);
  // Update new added sink PDO to USB PD engine
  usbpd1.updateSnkPdo();
}

void loop() {

}
