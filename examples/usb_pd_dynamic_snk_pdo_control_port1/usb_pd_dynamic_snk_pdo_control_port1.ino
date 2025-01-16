// usb_pd_dynamic_snk_pdo_control_port1.ino
// This example code is available for PMGDuino only
// This example code demonstrates the functionalities of
// (1) How to initiate the USB-C port 1
// (2) How to register USB PD event
// (3) How to change sink PDO in run-time
// (4) How to get the USB-C port 1 status
// (5) How to get the requested PDO index
void pwrNegotiationCompleted(byte portIndex)
{
  if (portIndex == 1)
  {
    // Update port 1 status
    usbpd1.updateStatus();
    
    // If port power role is sink power role
    // get and print the requested PDO by partner port
    if (usbpd1.powerRole == CY_PD_PRT_ROLE_SINK)
      Serial.print(usbpd1.getCurrentSnkPdo());
  }   
}

void setup() {
  // Initiate Serial interface
  Serial.begin(9600);
  // Initiate USB port 1
  usbpd1.begin();
  // Register APP_EVT_PD_CONTRACT_NEGOTIATION_COMPLETE event 
  usbpd1.registerEvent(APP_EVT_PD_CONTRACT_NEGOTIATION_COMPLETE, pwrNegotiationCompleted);
  // Set D15 as input pull-up mode
  pinMode(D15, INPUT_PULLUP);
}

byte pdoCnt = 5;
byte pdoCnt_old = 5;

void loop() {
  if (pdoCnt != pdoCnt_old)
  {
    changePdoCnt();
    pdoCnt_old = pdoCnt;
  }

  if (digitalRead(D15) == 0)
  {
    // If button is pressed (active low), de-bounce it

    // Turn-on built-in LED when the button is pressed
    digitalWrite(LED_BUILTIN, 0);

    // Debounce the button pressing 
    delay(300);
    if (digitalRead(D15) == 0)
    {
      // Change number of Source PDO count
      pdoCnt ++;
      if (pdoCnt >= 6)
        pdoCnt = 0;

      // Wait the button to be relased
      while(digitalRead(D15) == 0) {}
      // Turn-off built-in LED when the button is released
      digitalWrite(LED_BUILTIN, 1);
    }
  }
}

void changePdoCnt(void)
{
  byte numPdo = usbpd1.getSnkPdoNum();
  
  for (byte i = 1; i <= numPdo; i ++)
    usbpd1.removeSnkPdo(i);
  
  switch(pdoCnt)
  {
    // Enable sink EPR PDO
    case 5: usbpd1.enableSnkEprPdo(true); break;
    // Add 20V/3A to sink PDO list
    case 4: usbpd1.addFixedSnkPdo(4, 20000, 3000);
    // Add 15V/3A to sink PDO list
    case 3: usbpd1.addFixedSnkPdo(3, 15000, 3000);
    // Add 12V/3A to sink PDO list
    case 2: usbpd1.addFixedSnkPdo(2, 12000, 3000);
    // Add 9V/3A to sink PDO list
    case 1: usbpd1.addFixedSnkPdo(1, 9000, 3000);
    // Disable sink EPR PDO
    case 0: usbpd1.enableSnkEprPdo(false); 
            break;
    default: break;
  }
  // Update changed sink PDOs to USB PD engine
  usbpd1.updateSnkPdo();
}