void SPIClass::begin()
{
	spiPinInit();
}

void SPIClass::end()
{
	spiDeInit();
}

void SPIClass::beginTransaction(SPISettings settings)
{
	spiParameter.clockFreq = checkValidBitRate(settings.clockFreq);
	spiParameter.bitOrder = checkValidBitOrder(settings.bitOrder);
	spiParameter.dataMode = checkValidSpiMode(settings.dataMode);

	uint8_t div = SPI_SYSTEM_RATE / spiParameter.clockFreq / SPI_OVERSAMPLE_COUNT;
	if (spiInit(div, spiParameter.bitOrder, spiParameter.dataMode))
		spiHwInitaited = true;
	else
		spiHwInitaited = false;
}

void SPIClass::endTransaction(void)
{
	spiDisable();
	spiHwInitaited = false;
}

void SPIClass::setBitOrder(uint8_t order)
{
	if (checkValidBitOrder(spiParameter.bitOrder) != order)
	{
		spiParameter.bitOrder = checkValidBitOrder(order);
		if (spiHwInitaited)
		{
			spiDeInit();
			delay(1);
			uint8_t div = SPI_SYSTEM_RATE / spiParameter.clockFreq / SPI_OVERSAMPLE_COUNT;
			spiInit(div, spiParameter.bitOrder, spiParameter.dataMode);
		}
	}
}

void SPIClass::setDataMode(uint8_t mode)
{
	if (checkValidSpiMode(spiParameter.dataMode) != mode)
	{
		spiParameter.dataMode = checkValidSpiMode(mode);
		if (spiHwInitaited)
		{
			spiDeInit();
			delay(1);
			uint8_t div = SPI_SYSTEM_RATE / spiParameter.clockFreq / SPI_OVERSAMPLE_COUNT;
			spiInit(div, spiParameter.bitOrder, spiParameter.dataMode);
		}
	}
}

void SPIClass::setClockDivider(uint8_t div)
{
	if ((div > 128) || (div < 2) || (div % 2))
		return;

	uint32_t newRate = SPI_SYSTEM_RATE / div / SPI_OVERSAMPLE_COUNT;

	if (checkValidBitRate(newRate) != spiParameter.clockFreq)
	{
		spiParameter.clockFreq = checkValidBitRate(newRate);
		if (spiHwInitaited)
		{
			spiDeInit();
			delay(1);
			spiInit(div, spiParameter.bitOrder, spiParameter.dataMode);
		}
	}
}

uint8_t SPIClass::transfer(uint8_t val)
{
	uint8_t txData, rxData = 0x00;

	txData = val;

	if (spiHwInitaited)
	{
		spiSendData(&txData, &rxData, 1);
	}

	return rxData;
}

void SPIClass::transfer(uint8_t* buffer, uint8_t size)
{
	uint8_t i;
	uint8_t rxData;
	
	if(spiHwInitaited)
	{
		for (i=0; i<size; i++)
		{
			spiSendData(&buffer[i], &rxData, 1);
			buffer[i] = rxData;
		}
	}
	else
	{
		memset((void *)buffer, 0, (size_t)size);
	}
}

uint16_t SPIClass::transfer16(uint16_t val16)
{
	uint16_t rx_buffer = 0;
	uint8_t tx_buf[2] = {0, 0};

	if (spiHwInitaited)
	{
		if (spiParameter.bitOrder == LSBFIRST)
		{
			tx_buf[0] = val16;
			tx_buf[1] = (val16>>8);
		}
		else
		{
			tx_buf[0] = (val16>>8);
			tx_buf[1] = val16;
		}


		spiSendData(tx_buf, (uint8_t *)&rx_buffer, 2);
	}

	return rx_buffer;
}

SPIClass SPI=SPIClass();
