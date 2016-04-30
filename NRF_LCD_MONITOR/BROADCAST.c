/*
   CAUTION: This uses the arduino nano
   Arduino Nano
   Broadcasts ADC readings over the air
   
   
   Author: pocketaces
   Date: 20160430
*/

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

uint32_t *packetaddr;
uint32_t packetdata;

void setup(){
   // Initialize values
   packetdata = 0;
   packetaddr = &packetdata;
   
   Mirf.csnPin = 7;
   Mirf.cePin = 8;
   Mirf.channel = 10;
   Mirf.spi = &MirfHardwareSpi;
   Mirf.init();
   Mirf.setRADDR((byte *)"mast1"); // My address
   Mirf.setTADDR((byte *)"node1"); //Who to send to?
   Mirf.payload = sizeof(packetdata);
   Mirf.config();
   
}

void loop(){
   Mirf.send((byte *) packetaddr);
   while(Mirf.isSending());
   
   packetdata = (uint32_t ) analogRead(A0);
   // packetdata++; // for debugging use
   delay(500);
}