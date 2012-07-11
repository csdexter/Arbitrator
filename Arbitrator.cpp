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
  TSPIConfiguration *config =
    (TSPIConfiguration*)_topology[client].configuration;

  SPI.setBitOrder(config->byteOrder);
  SPI.setDataMode(config->dataMode);
  SPI.setClockDivider(config->clockDivider);
};

const TSPIConfiguration a = { MSBFIRST, SPI_CLOCK_DIV8, SPI_MODE0 };
const TBusTopology b = { ARBITRATOR_MODE_MASTER, 1, (byte*)&a };