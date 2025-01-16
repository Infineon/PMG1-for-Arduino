// usb_pd_dynamic_src_pdo_control_port1.ino
// This example code is available for PMGDuino only
// This example code demonstrates the functionalities of
// (1) How to initiate the USB-C port 1
// (2) How to register USB PD event
// (3) How to change source PDO in run-time
// (4) How to get the USB-C port 1 status
// (5) How to get the requested PDO index
// (6) How to do the power role swap in the application
// (7) How to enable or disable source EPR PDO in run-time
void pwrNegotiationCompleted(byte portIndex)
{
  if (portIndex == 1)
  {
    // Update port 1 status
    usbpd1.updateStatus();
    
    // If port power role is sink power role
    // then request power role swap
    // else get and print the requested PDO by partner port
    if (usbpd1.powerRole == CY_PD_PRT_ROLE_SINK)
      usbpd1.doPwrRoleSwap();
    else
      Serial.print(usbpd1.getCurrentSrcRdo());
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

  // Check if partner is supported and entered EPR mode successfully
  // if yes, turn on LED 1, otherwise turn off LED1
  if (usbpd1.getEprModeActive())
    digitalWrite(LED1, 0);
  else
    digitalWrite(LED1, 1);
}

void changePdoCnt(void)
{
  byte numPdo = usbpd1.getSrcPdoNum();
  
  for (byte i = 1; i <= numPdo; i ++)
    usbpd1.removeSrcPdo(i);
  
  switch(pdoCnt)
  {
    // Enable source EPR PDO
    case 5: usbpd1.enableSrcEprPdo(true); break;
    // Add 20V/3A to source PDO list
    case 4: usbpd1.addFixedSrcPdo(4, 20000, 3000, 0);
    // Add 15V/3A to source PDO list
    case 3: usbpd1.addFixedSrcPdo(3, 15000, 3000, 0);
    // Add 12V/3A to source PDO list
    case 2: usbpd1.addFixedSrcPdo(2, 12000, 3000, 0);
    // Add 9V/3A to source PDO list
    case 1: usbpd1.addFixedSrcPdo(1, 9000, 3000, 0);
    // Disable source EPR PDO
    case 0: usbpd1.enableSrcEprPdo(false); 
            break;
    default: break;
  }
  // Update changed source PDOs to USB PD engine
  usbpd1.updateSrcPdo();
}