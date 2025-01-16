// usb_pd_simple_src_pdo_example_port1.ino
// This example code is available for PMGDuino only
// This example code demonstrates the functionalities of
// (1) How to initiate the USB-C port 1 
// (2) How to add new source PDOs in run-time
void setup() {
  // Initiate USB-C port 1
  usbpd1.begin();
  // Add 9V/3A, 12V/3A, 15V/3A and 20V/3A fixed PDO
  usbpd1.addFixedSrcPdo(1, 9000, 3000, 0);
  usbpd1.addFixedSrcPdo(2, 12000, 3000, 0);
  usbpd1.addFixedSrcPdo(3, 15000, 3000, 0);
  usbpd1.addFixedSrcPdo(4, 20000, 3000, 0);
  // Update new added sink PDO to USB PD engine
  usbpd1.updateSrcPdo();
}

void loop() {

}
