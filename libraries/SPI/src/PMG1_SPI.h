#ifndef SPI_H_
#define SPI_H_

#define checkValidBitRate(rate) (((uint32_t)rate > 1500000 || (uint32_t)rate < 23437) ? (uint32_t)1000000 : (uint32_t)rate)
#define checkValidBitOrder(order) (((uint8_t)order > 1) ? (uint8_t)MSBFIRST : (uint8_t)order)
#define checkValidSpiMode(mode) (((uint8_t)mode > 3) ? (uint8_t)SPI_MODE0 : (uint8_t)mode)

#ifdef __cplusplus

typedef enum {
  SPI_MODE0 = 0,
  SPI_MODE1 = 1,
  SPI_MODE2 = 2,
  SPI_MODE3 = 3,
} SPIMode;

#if (0)
typedef enum {
  LSBFIRST = 0,
  MSBFIRST = 1,
} BitOrder;
#endif

class SPISettings {
  public:
  SPISettings(uint32_t speedMaximum, uint8_t dataOrder, uint8_t dataMode)
  {
    clockFreq   = speedMaximum;
    bitOrder    = dataOrder;
    dataMode    = dataMode;
  }

  SPISettings()
  {
    clockFreq   = 1000000;
    bitOrder    = MSBFIRST;
    dataMode    = SPI_MODE0;
  }

  uint32_t  clockFreq;
  uint8_t   bitOrder;
  uint8_t   dataMode;
};


class SPIClass
{
private:
  bool spiHwInitaited = false;

public:
  SPISettings spiParameter;

  SPIClass()
  {
    spiParameter = SPISettings();
  }

  void begin();
  void end();

  void setBitOrder(uint8_t order);
  void setDataMode(uint8_t mode);
	void setClockDivider(uint8_t div);

  void beginTransaction(SPISettings settings);
	void endTransaction(void);
	  

  uint8_t transfer(uint8_t val);
  void transfer(uint8_t* buffer, uint8_t size);
  uint16_t transfer16(uint16_t val16);
};

extern SPIClass SPI;

#endif /*__cplusplus*/

#endif /* SPI_CLASS_TEST_H_ */
