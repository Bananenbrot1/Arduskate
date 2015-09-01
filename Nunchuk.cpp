/*
 * Based on ArduinoNunchuk.h changed wrong types removed unused Axes
 * 2014 Barney
 * ArduinoNunchuk.cpp - Improved Wii Nunchuk library for Arduino
 * 
 * Copyright 2011-2012 Gabriel Bianconi, http://www.gabrielbianconi.com/
 *
 * Project URL: http://www.gabrielbianconi.com/projects/arduinonunchuk/
 *
 * Based on the following projects/websites:
 *   http://www.windmeadow.com/node/42
 *   http://todbot.com/blog/2008/02/18/wiichuck-wii-nunchuck-adapter-available/
 *   http://wiibrew.org/wiki/Wiimote/Extension_Controllers
 * 
 */

#if (ARDUINO >= 100)
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <Wire.h>
#include "Nunchuk.h"

#define ADDRESS 0x52

void Nunchuk::init()
{ 

  byte cnt;   
  Wire.begin();
  // Initialize
  Nunchuk::_sendByte(0x55, 0xF0);
  delay(1);
  Nunchuk::_sendByte(0x00, 0xFB);
  delay(1);

  Wire.beginTransmission(0x52);
  Wire.write(0xFA);                    // extension type register
  Wire.endTransmission();
  Wire.beginTransmission(0x52);
  Wire.requestFrom(0x52, 6);               // request data from controller
  for (cnt = 0; cnt < 6; cnt++) {
     if (Wire.available()) {
         ctrlr_type[cnt] = Wire.read(); // Should be 0x0000 A420 0101 for Classic Controller, 0x0000 A420 0000 for nunchuck
     }
  }
  Wire.endTransmission();
  delay(1);
             
  // send the crypto key (zeros), in 3 blocks of 6, 6 & 4.
  Wire.beginTransmission(0x52);
  Wire.write(0xF0);                    // crypto key command register
  Wire.write(0xAA);                    // sends crypto enable notice
  Wire.endTransmission();
  delay(1);
  Wire.beginTransmission(0x52);
  Wire.write(0x40);                    // crypto key data address
  for (cnt = 0; cnt < 6; cnt++) {
     Wire.write(0x00);                    // sends 1st key block (zeros)
  }
  Wire.endTransmission();
  Wire.beginTransmission(0x52);
  Wire.write(0x40);                    // sends memory address
  for (cnt = 6; cnt < 12; cnt++) {
     Wire.write(0x00);                    // sends 2nd key block (zeros)
  }
  Wire.endTransmission();
  Wire.beginTransmission(0x52);
  Wire.write(0x40);                    // sends memory address
  for (cnt = 12; cnt < 16; cnt++) {
     Wire.write(0x00);                    // sends 3rd key block (zeros)
  }
  Wire.endTransmission();
  delay(1);
  // end device init
}
    
void Nunchuk::update()
{ 
  byte count = 0;      
  unsigned int values[6];
  // crypto key data address
  Nunchuk::_sendByte(0x40, 0x00); 
  
  Wire.requestFrom (ADDRESS, 6); 
  
  while(Wire.available())
  {
    values[count] = Wire.read();
    count++;
  }
  Nunchuk::analogX = values[0];
  Nunchuk::analogY = values[1];      
  Nunchuk::accelX = values[2] * 2 * 2 + ((values[5] >> 2) & 1) * 2 + ((values[5] >> 3) & 1);
  Nunchuk::accelY = values[3] * 2 * 2 + ((values[5] >> 2) & 1) * 2 + ((values[5] >> 3) & 1);
  Nunchuk::accelZ = values[4] * 2 * 2 + ((values[5] >> 2) & 1) * 2 + ((values[5] >> 3) & 1);
  Nunchuk::zButton = !((values[5] >> 0) & 1);
  Nunchuk::cButton = !((values[5] >> 1) & 1);
  
  Nunchuk::_sendByte(0x00, 0x00);
}
  
void Nunchuk::_sendByte(byte data, byte location)
{  
  Wire.beginTransmission(ADDRESS);
  
  #if (ARDUINO >= 100)
    Wire.write(location);
    Wire.write(data);  
  #else
    Wire.send(location);
    Wire.send(data); 
  #endif
  
  Wire.endTransmission();
  
  delay(10);
}