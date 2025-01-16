#ifndef TwoWire_h
#define TwoWire_h

#ifdef __cplusplus

#include "Stream.h"

//#define BUFFER_LENGTH 32

// WIRE_HAS_END means Wire has end()
#define WIRE_HAS_END 1

class TwoWire : public Stream
{
  private:
    //uint32_t i2cTimeout; // 25ms
    //bool i2cResetTimeout;
    
    //static uint8_t rxBuffer[];
    //static uint8_t rxBufferIndex;
    //static uint8_t rxBufferLength;

    //static uint8_t txAddress;
    //static uint8_t txBuffer[];
    //static uint8_t txBufferIndex;
    //static uint8_t txBufferLength;

    //static uint8_t transmitting;
    //static void (*user_onRequest)(void);
    //static void (*user_onReceive)(int);
    //static void onRequestService(void);
    //static void onReceiveService(uint8_t*, int);
    //static void slaveStatus(void);

    bool twoWireInitiazed;
  public:
    TwoWire();
    void begin();
    void begin(uint8_t);
    void begin(int);
    void end();
    void setClock(uint32_t rate);
    //void setWireTimeout(uint32_t timeout = 25000, bool reset_with_timeout = true);
    //bool getWireTimeoutFlag(void);
    //void clearWireTimeoutFlag(void);
    void beginTransmission(uint8_t address);
    void beginTransmission(int address);
    uint8_t endTransmission(void);
    uint8_t endTransmission(uint8_t sendStop);
    uint8_t requestFrom(uint8_t address, uint8_t quantity);
    uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop);
    uint8_t requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop);
    uint8_t requestFrom(int address, int quantity);
    uint8_t requestFrom(int address, int quantity, int sendStop);
    virtual size_t write(uint8_t data);
    virtual size_t write(const uint8_t *data, size_t quantity);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    void onReceive(void (*function)(int));
    void onRequest(void (*function)(void));

    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write;
};

extern TwoWire Wire;
#endif

#endif