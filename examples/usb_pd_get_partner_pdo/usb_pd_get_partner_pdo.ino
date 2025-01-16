// usb_pd_get_partner_pdo.ino
// This example demonstrates the functionalities
// This example is available for both CY7113 and PMGDuino
// (1) How to initiate the USB PD port 0
// (2) How to register and use USB PD event
// (3) How to retrieve the source/sink PDOs from the partner port

void printOutPdos(void)
{
    // Update USB-C port 0 status
    usbpd0.updateStatus();

    // Print out the current power mode
    if (usbpd0.powerRole == CY_PD_PRT_ROLE_SOURCE)
      Serial.println("Src_Pwr_Role");
    else if (usbpd0.powerRole == CY_PD_PRT_ROLE_SINK)
      Serial.println("Snk_Pwr_Role");

    // Get the sink PDOs from partner port
    usbpd0.getPartnerPortSnkPdo();
    // Wait until the traffic is accomplished
    while(!usbpd0.iPartnerSnkPdoUpdated) {}

    // If the port is not as a sink power role
    // then get the source PDOs from partner port
    // this check is for avoiding the power renegotiation 
    if (usbpd0.powerRole != CY_PD_PRT_ROLE_SINK)
    {
      // Get the source PDOs from partner port
      usbpd0.getPartnerPortSrcPdo();
      // Wait until the traffic is accomplished
      while(!usbpd0.iPartnerSrcPdoUpdated) {}
    }

    // Print out the source PDOs of the partner port
    Serial.println("Src PDOs");

    for (byte i = 0; i < usbpd0.iPartnerSrcPdoCnt; i ++)
    {
      // Print out the PDO value
      Serial.println(usbpd0.iPartnerSrcPdo[i].val);
      // Delay for a while for printing
      delay(50); 
    }

    // Print out the sink PDOs of the partner port
    Serial.println("Snk PDOs");

    for (byte i = 0; i < usbpd0.iPartnerSnkPdoCnt; i ++)
    {
      // Print out the PDO value
      Serial.println(usbpd0.iPartnerSnkPdo[i].val);
      // Delay for a while for printing
      delay(50);
    }

}

void pwrNegotiationCompleted(byte portIndex)
{
  if (portIndex == 0)
  {
    // Negotiation is completed
    Serial.println("Done");
    printOutPdos();
  }   
}

void setup() {
  // Initiate the Serial interface
  Serial.begin(9600);
  // Initiate the USB-C port 0
  usbpd0.begin();
  // Register the event
  usbpd0.registerEvent(APP_EVT_PD_CONTRACT_NEGOTIATION_COMPLETE, pwrNegotiationCompleted);
}

bool skip = false;

void loop() {
  // put your main code here, to run repeatedly:
}
