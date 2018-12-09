#include <stdio.h>
#include <stdint.h>
#include <Wire.h>

#define MAC_24AA02Exx_DEV_ADDR        0x50
#define MAC_24AA02Exx_REG_ADDR_MAC    0xF8

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // execute ADC one time in order to power the Arduino AREF pin
  analogRead(A0);
  delay(1);  // Wait until the IC starts up

  // I2C init and read MAC address
  Wire.begin();
  Wire.setClock(100000);
  Wire.beginTransmission(MAC_24AA02Exx_DEV_ADDR);
  Wire.write(MAC_24AA02Exx_REG_ADDR_MAC);
  Wire.endTransmission(false);
  Wire.requestFrom(MAC_24AA02Exx_DEV_ADDR, 8);
  uint8_t MAC_addr_buf[8] = {'\0'};
  for(int i = 0; i < 8; i++)
    MAC_addr_buf[i] = Wire.read();
  
  // Determine whether the MAC address is EUI-48 or EUI-64
  int MAC_bytes;
  uint8_t MAC_addr[8] = {'\0'};
  if( (MAC_addr_buf[0] == 0xFF) && (MAC_addr_buf[1] == 0xFF) ) {
    MAC_bytes = 6;  // EUI-48 (24AA02xE48)
    memcpy(MAC_addr, MAC_addr_buf + 2, MAC_bytes);
  } else {
    MAC_bytes = 8;  // EUI-64 (24AA02xE64)
    memcpy(MAC_addr, MAC_addr_buf, MAC_bytes);
  }

  // print MAC address
  Serial.print(8 * MAC_bytes);
  Serial.print("bit MAC address : ");
  for(int i = 0; i < MAC_bytes; i++) {
    char strBuf[3];
    sprintf(strBuf, "%02X", MAC_addr[i]);
    Serial.print(strBuf);
    if(i < (MAC_bytes - 1))
      Serial.print("-");
  }
  Serial.println();
}

void loop() {
}
