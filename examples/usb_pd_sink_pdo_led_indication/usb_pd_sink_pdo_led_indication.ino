// usb_pd_simple_sink_pdo_example.ino
// This example code is available for both CY7113 and PMGDuino
// This example code demonstrates the functionalities of
// (1) How to initiate the USB-C port 0 
// (2) How to indicate the accpeted PDO by different LED blinking period
void setup() {
  // put your setup code here, to run once:

  // Startup the USB PD port 0
  usbpd0.begin();
  
  // Add Sink PDO 9V/3A to Sink PDO index 1
  usbpd0.addFixedSnkPdo(1, 9000, 3000);
  // Add Sink PDO 9V/3A to Sink PDO index 2
  usbpd0.addFixedSnkPdo(2, 15000, 3000);
  // Add Sink PDO 9V/3A to Sink PDO index 3
  usbpd0.addFixedSnkPdo(3, 20000, 3000);
  // Update the new added Sink PDOs to the USB PD engine
  usbpd0.updateSnkPdo();
}

void loop() {
  // put your main code here, to run repeatedly:

  unsigned int delay_period;
  byte pdoIdx;

  // Get the current accepted Source PDO request
  pdoIdx = usbpd0.getCurrentSnkPdo();

  // 
  switch (pdoIdx)
  {
    case 0: delay_period = 500; break; // 5V/3A
    case 1: delay_period = 250; break; // 9V/3A
    case 2: delay_period = 125; break; // 15V/3A
    case 3: delay_period = 75; break;  // 20V/3A
    default: delay_period = 2000; break; // No USB PD contact established
  }

  // Blinking the LED in specific period to identify the USB PD power status
  digitalWrite(LED_BUILTIN, 0);
  delay(delay_period);
  digitalWrite(LED_BUILTIN, 1);
  delay(delay_period);
}
