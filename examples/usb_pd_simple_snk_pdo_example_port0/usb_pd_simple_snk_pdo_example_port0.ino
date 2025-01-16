// usb_pd_simple_snk_pdo_example_port0.ino
// This example code is available for both CY7113 and PMGDuino
// This example code demonstrates the functionalities of
// (1) How to initiate the USB-C port 0 
// (2) How to add new sink PDOs in run-time
void setup() {
  // Initiate USB-C port 0
  usbpd0.begin();
  // Add 9V/3A, 12V/3A, 15V/3A and 20V/3A fixed PDO
  usbpd0.addFixedSnkPdo(1, 9000, 3000);
  usbpd0.addFixedSnkPdo(2, 12000, 3000);
  usbpd0.addFixedSnkPdo(3, 15000, 3000);
  usbpd0.addFixedSnkPdo(4, 20000, 3000);
  // Update new added sink PDO to USB PD engine
  usbpd0.updateSnkPdo();
}

void loop() {

}
