# 64bit MACアドレスモジュール

<img src="/images/ProductImage.jpg" width="300px">

## 概要

64bit MACアドレスモジュールはMicrochip社のI2C接続のMACアドレス書き込み済みEEPROM 24AA025E64を2.54mmピッチに変換したモジュールです。  
IPv6、ZigBee、FireWire等のネットワーク接続デバイスに一意の64bit MACアドレス(EUI-64)を割り当てることができます。  
ピンヘッダ含め部品は全て実装済みのため**はんだ付け作業は一切不要**です。また、**Arudinoシールド等に直接挿せるピン配置**となっているため、ジャンパワイヤ等を使わず手軽に接続できます。  
**MACアドレスを記載したカードを同梱。**

※48bit MACアドレス版は[こちら](https://github.com/y2kblog/48bit_MAC_Address_Module)

### 仕様
- 搭載IC：Microchip 24AA025E64T-I/OT
- メモリ容量：2Kbits (内ユーザ書込み可能領域：1Kbits)
- インターフェイス：I2C (最大400kHz)
- I2Cアドレス：0x50
- 電源電圧：1.7V～5.5V
- 最大消費電流
    - 読込み時：1mA
    - 書込み時：3mA
    - 待機時：1μA
- 基板サイズ：横10.0 mm × 縦7.5 mm

### 内容物
- 64bit MACアドレスモジュール本体
- MACアドレス記載カード

<img src="/images/ProductContents_64bit.jpg" width="300px">


## 販売  
[スイッチサイエンス委託販売ページ](https://www.switch-science.com/catalog/5319/)  
※大量注文や在庫に関する問い合わせは[こちら](mailto:info.y2kb@gmail.com)までご連絡ください。  

### 回路図

<img src="/images/CircuitDiagram.png" width="300px">


### Arduinoによるプログラム例

#### 接続方法  

Arudino UNOの場合、下の写真のように挿すだけでOKです。

<img src="/images/WiringDiagram_Arduino.jpg" width="400px">


#### ミニマムコード

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


### Nucleoによるプログラム例

#### 接続方法  

<img src="/images/WiringDiagram_Nucleo.jpg" width="350px">

#### ソースコード

NUCLEO-L476RGのソースコードは[こちら](https://github.com/y2kblog/64bit_MAC_Address_Module/blob/master/SampleCode/NUCLEO-L476RG/24AA025Exx_testing) (IDE：SW4STM32)


## License
MIT License, see LICENSE.txt.
