/* Arduino HAL Library - Bus Arbitration
 * See the README file for author and licensing information. In case it's
 * missing from your distribution, use the one here as the authoritative
 * version: https://github.com/csdexter/HAL/blob/master/README
 *
 * This library deals with accessing busses (SPI, I2C etc.) as common resources,
 * i.e. from more than one place in the code and without causing conflicts.
 * See the example sketches to learn how to use the library in your code.
 *
 * This is the main code file for the library.
 * See the header file for better function documentation.
 * ---------------------------------------------------------------------------
 */

#include "Arbitrator.h"

#include <SPI.h>
#include <Wire.h>

void TSPIArbitrator::begin(const TBusTopology *topology, const byte length) {
  TBusArbitrator::begin(topology, length);
  _lastclient = length;
  SPI.begin();
};

void end() {
  SPI.end();
};

byte TSPIArbitrator::transfer(byte client, byte data) {
  switchToClient(client);

  return SPI.transfer(data);
};

void TSPIArbitrator::switchToClient(byte client) {
  if(client != _lastclient) {
    TSPIConfiguration *config =
      (TSPIConfiguration*)_topology[client].configuration;

    SPI.setBitOrder(config->byteOrder);
    SPI.setDataMode(config->dataMode);
    SPI.setClockDivider(config->clockDivider);
    
    _lastclient = client;
  };
};

void TI2CArbitrator::requestFrom(byte client, byte address, byte size,
                                 boolean stop) {
  Wire.requestFrom(address, size, stop);
};

void TI2CArbitrator::beginTransmission(byte client, byte address) {
  Wire.beginTransmission(address);
};
    
byte TI2CArbitrator::endTransmission(byte client, boolean stop) {
  return Wire.endTransmission(stop);
};

byte TI2CArbitrator::write(byte client, byte value) {
  return Wire.write(value);
};

byte TI2CArbitrator::write(byte client, const char *string) {
  return Wire.write(string);
};
    
byte TI2CArbitrator::write(byte client, const byte *data, word length) {
  return Wire.write(data, length);
};

word TI2CArbitrator::available(byte client) {
  return Wire.available();
};
    
byte TI2CArbitrator::read(byte client) {
  return Wire.read();
};

void TI2CArbitrator::begin(const TBusTopology *topology, const byte length) {
  TBusArbitrator::begin(topology, length);
  
  Wire.begin();
};

void TI2CArbitrator::end() {
  return;
};
