/*
 * Listens on NRF24L01 radio. Displays messages it receives
 * Author: pocketaces
 * Date:    2016 04 30
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#define I2C_ADDR	0x3F
#define En_pin		2
#define Rw_pin		1
#define Rs_pin		0
#define D4_pin		4
#define D5_pin		5
#define D6_pin		6
#define D7_pin		7
#define BACKLIGHT_PIN   3

LiquidCrystal_I2C lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin,
                      D4_pin, D5_pin, D6_pin, D7_pin);
int counter;
uint32_t * packetaddr;
uint32_t   packetdata;

void setup() {
   // Initialize variables
   packetdata = 0L;
   packetaddr = &packetdata;
   counter = 0;
   
	// Set up LCD
   lcd.begin (20, 4);
	// Switch on the backlight
	lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
	lcd.setBacklight(HIGH);
	// Position cursor and write some text
   lcd.clear();
	lcd.home();
	lcd.print("Hello World");
	delay(1000);
   // Done setting up LCD
   
   // Set up NRF24L01
   Mirf.csnPin = 7;
   Mirf.cePin = 8;
   Mirf.channel = 10;
   Mirf.spi = &MirfHardwareSpi;
   Mirf.init();
   Mirf.setRADDR((byte *)"node1"); // My address
   Mirf.setTADDR((byte *)"mast1"); //Who to send to?
   
   Mirf.payload = sizeof(long);
   Mirf.config();
   lcd.clear();
   lcd.home();
   lcd.print("Radio ready");
   delay(1000);
   // Radio Ready
}

void loop() {
   do{
      if (!Mirf.isSending() && Mirf.dataReady()){
         Mirf.getData((byte *) packetaddr);
         counter = 0;   //reset the lag counter if a packet was received
         lcd.clear();
         lcd.home();
         lcd.print(String(packetdata));
      }
   } while (!Mirf.rxFifoEmpty());
   
	//lcd.clear();
	//lcd.home();
	//lcd.print(counter);
	counter++;  //increment the lag counter
	delay(300);
   if (counter>10){
      lcd.clear();
      lcd.home();
      lcd.print("No packet...");
      counter = 10;
   }
}
