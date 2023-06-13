#include "xbeeHelper.h"

//https://www.digi.com/resources/documentation/Digidocs/90002002/Content/Tasks/t_calculate_checksum.htm?TocPath=API%20Operation%7CAPI%20frame%20format%7C_____1
void add_cksum(byte p[], int pktsize)
{
  int cksum = 0; // start with a zero checksum
  for (byte i = 3; i < pktsize - 1; i++)
  { // skip the Start and len, start with byte 4.
    cksum += p[i];
  }
  cksum &= 0xFF;        // low order 8 bits
  cksum = 0xFF - cksum; // subtract from 0xFF
  p[pktsize - 1] = cksum;
}

/*
    https://www.digi.com/resources/documentation/Digidocs/90001942-13/concepts/c_api_frame_structure.htm?tocpath=XBee%20API%20mode%7C_____2
    https://www.digi.com/resources/documentation/Digidocs/90002002/Content/Reference/r_api_frame_format_900hp.htm
    https://forum.arduino.cc/t/pass-reference-to-serial-object-into-a-class/483988/27
    frame api mode:
    
    writeHex packet[] set up for length 18 - 0x12 at 3rd byte (packet[2])
    represents the length - this means we can send 4 bytes.
    to send 5 btyes change 0x12 to 0x13 (lenght 19) and add an 0x00 to the end of packet[]
    now you could and send packet[21] = ...; or go the other way to send fewer bytes
    this  is set up for my coordinator radio you must change bytes packet[9] - packet[12]
    0x40,0x8B,0x2D,0x4C to the address of your coordinator radio. Also check your full address
    all my S2 radios have same lower bytes 0013A200 - I don't know if S3 radios use the same
    There is a ZBTxRequest object in the xbee library to avoid having to build your own
    packets like I do here - this would not work for me. Let me know if you know how...
    https://github.com/andrewrapp/xbee-arduino/blob/master/examples/Series2_Tx/Series2_Tx.pde
    paramater: data[]
    data[0] r
    data[1] g
    data[2] b
    data[4] pin
*/

void writeHex(byte data[],Stream &serial ){
  byte packet[] = {0x7E,0x00,0x12,0x10,0x01,0x00,0x13,0xA2,0x00,0x40,0x8B,0x2D,0x4C,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  packet[17] = data[0];
  packet[18] = data[1];
  packet[19]=  data[2];
  packet[20] = data[3];
  add_cksum(packet,sizeof(packet));
  serial.write(packet,sizeof(packet));
}

/*
  length 16 - 0x10 at byte 3
  paramater: data[]
  data[0] pin state 0,1
  data[1] pin
*/

void writePinState(byte data[], Stream  &serial){
  byte packet[] = {0x7E,0x00,0x10,0x10,0x01,0x00,0x13,0xA2,0x00,0x40,0x8B,0x2D,0x4C,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00};
  packet[17] = data[0];
  packet[18] = data[1];
  add_cksum(packet,sizeof(packet));
  serial.write(packet,sizeof(packet));
}