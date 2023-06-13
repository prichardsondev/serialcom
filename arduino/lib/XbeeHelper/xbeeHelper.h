#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>

void add_cksum(byte p[], int pktsize);
void writePinState(byte data[], Stream &serial);
void writeHex(byte data[], Stream &serial);