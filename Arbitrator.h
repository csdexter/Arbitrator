/* Arduino HAL Library - Bus Arbitration
 * See the README file for author and licensing information. In case it's
 * missing from your distribution, use the one here as the authoritative
 * version: https://github.com/csdexter/HAL/blob/master/README
 *
 * This library deals with accessing busses (SPI, I2C etc.) as common resources,
 * i.e. from more than one place in the code and without causing conflicts.
 * See the example sketches to learn how to use the library in your code.
 *
 * This is the main include file for the library.
 * ---------------------------------------------------------------------------
 */

#ifndef _ARBITRATOR_H_INCLUDED
#define _ARBITRATOR_H_INCLUDED

#if defined(ARDUINO) && ARDUINO >= 100
# include <Arduino.h>  
#else
# include <WProgram.h>  
#endif

#define ARBITRATOR_MODE_MASTER 0x01
#define ARBITRATOR_MODE_SLAVE 0x02

typedef struct {
  byte mode, address;
  byte *configuration;
} TBusTopology;

class TBusArbitrator {
  public:
    /* system/bus wide initialization */
    virtual void begin(const TBusTopology *topology, const byte length) {
      _topology = topology;
      _nodes = length;
    };
    
    /* system/bus wide termination */
    virtual void end() = 0;
    
    /* underlying hardware currently servicing another request */
    virtual boolean isBusy() = 0;

  protected:
    const TBusTopology *_topology;
    byte _nodes;
    
    virtual void switchToClient(byte client) = 0;
};

typedef struct {
  byte clockDivider;
  byte dataMode;
  byte byteOrder;
} TSPIConfiguration;

class TSPIArbitrator: public TBusArbitrator {
  public:
    byte transfer(byte client, byte data);
    void begin(const TBusTopology *topology, const byte length);
    void end();

    /* SPI backend is blocking */
    boolean isBusy() { return false; };

  private:
    void switchToClient(byte client);
};

class TI2CArbitrator: public TBusArbitrator {
  public:
    void requestFrom(byte client, byte address, byte size,
                     boolean stop = true);
    void beginTransmission(byte client, byte address);
    byte endTransmission(byte client, boolean stop = true);
    byte write(byte client, byte value);
    byte write(byte client, const char *string);
    byte write(byte client, const void *data, word length);
    word available(byte client);
    byte read(byte client);
    void onReceive(byte client, void (*handler)(word));
    void onRequest(byte client, void (*handler)(void));
    void begin(const TBusTopology *topology, const byte length);
    void end() = 0;
    boolean isBusy();
  private:
    void switchToClient(byte client);
};

#endif
