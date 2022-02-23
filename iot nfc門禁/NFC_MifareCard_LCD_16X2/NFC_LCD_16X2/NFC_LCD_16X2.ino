
/*
 *Project name:
     ARDUINO IN HIGH SERIE 00 - NFC ITEAD PN532 & LCD!!!
     (The 'Hello World' example for the nfc Mifare Card)
     Hex File: NFC_MifareCArd_LCD_16X2.ino
  *Revision History:
     20160210:
       - found on internet (gilj3)
 *Description:
     let's play with the Contactless Mifarecard and  show you 
     how connect NFC Module ITEAD PN532 to the Arduino and
     to simulate a security access. Enjoy good,and have fun. Bye!!!
 *MCU:              Arduino - @16MHz           http://www.arduino.cc/
 *NFC.Board:       NFC Module ITEAD PN532      http://wiki.iteadstudio.com/ITEAD_PN532_NFC_MODULE
 *Connections:
     arduino pins 13,12,11,10, 5v, gnd ->  Itead's pins sdk, mi, mo, scl, 5v, gnd 
     Arduino pins 8, 9, gnd, gnd, gnd in LCD´pins rs, e, vss, rw, k (vo in mid pot)
 */
 
// --- libraries ---  //

#include <PN532.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#define SCK 13
#define MOSI 11
#define SS 10
#define MISO 12
LiquidCrystal lcd(7,6,5,4,3,2); 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED 寬度像素
#define SCREEN_HEIGHT 64 // OLED 高度像素
// 設定OLED
#define OLED_RESET 4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
/*Chip select pin can be connected to D10 or D9 which is hareware optional*/
/*if you the version of NFC Shield from SeeedStudio is v2.0.*/
#define PN532_CS 10
PN532 nfc(PN532_CS);
#define  NFC_DEMO_DEBUG 1
// --- Configurations---  //
void setup()
{
  display.display();
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0,0);
  Serial.println("Hello!");
  lcd.print("Hello!");
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
  #ifdef NFC_DEMO_DEBUG
    Serial.println("Didn't find PN53x board");
  #endif
    while (1); // halt
  }

  // configure board to read RFID tags and cards
  nfc.SAMConfig();
  init_display();

}

// --- print method---  //

void init_display(){
  // show time once in a while
  lcd.clear();
  lcd.setCursor(0,0);
  Serial.println("Hi!! Pass Card!!");
  lcd.print("hi! pass card!!");
}
void testdrawstyles(void) {
  display.clearDisplay();
  display.setTextSize(4);             // 設定文字大小
  display.setTextColor(1);        // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
  display.setCursor(0,0);             // 設定起始座標
  display.print("CGU  STORE");        // 要顯示的字串
  display.display();                  // 要有這行才會把文字顯示出來
  delay(1000);
}
void loop() {
  uint32_t id;
  id=nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A);

  if(id!=0){
   
    if(id==83871787 ){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Hi Denny");
      lcd.setCursor(0,1);
      lcd.print("Welcomed cgu");
      delay(1500);
      init_display();
    }
    else if(id==3645227185)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Hi Jason");
      lcd.setCursor(0,1);
      lcd.print("Welcomed cgu");
      delay(1500);
      init_display();
    }
   else if(id==2299670715)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Hi Melody");
      lcd.setCursor(0,1);
      lcd.print("Welcomed cgu");
      Serial.print("cgu hi");
      delay(1500);
      init_display();
    }
  }
   
}
