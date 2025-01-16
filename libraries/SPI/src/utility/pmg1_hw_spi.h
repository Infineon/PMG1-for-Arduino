

void spiPinInit(void);
bool spiInit(uint8_t div, bool msbFirst, uint8_t mode);
void spiDeInit(void);
void spiDisable(void);
void spiSetPeripheralClock(uint8_t div);
void spiSendData(uint8_t *txData, uint8_t *rxData, uint8_t size);
