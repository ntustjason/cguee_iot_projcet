6#include <PN532.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <MySQL_Encrypt_Sha1.h>
#include <MySQL_Packet.h>
#include <SoftwareSerial.h>
#define SCK (13)
#define MOSI (11)
#define SS (10)
#define MISO (12)
#define SCREEN_WIDTH 128 // OLED 寬度像素
#define SCREEN_HEIGHT 64 // OLED 高度像素
// 設定OLED
#define OLED_RESET  4// Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define PN532_CS 10
PN532 nfc(PN532_CS);
#define  NFC_DEMO_DEBUG 1
//自訂圖形，從Image2LCD轉過來的，尺寸是128x48，少掉的16個像素是上方黃色區塊(假設是用藍黃雙色的OLED)
//本範例為了在上方區塊顯示文字，所以圖形就沒有涵蓋整個高度

void setup(){
  // 清除畫面
 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 一般1306 OLED的位址都是0x3C
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
   Serial.begin(9600);
   delay(1000); 
   display.clearDisplay();
   testdrawstyles();
   nfc.begin();
   uint32_t versiondata = nfc.getFirmwareVersion();
   if (! versiondata) {
   #ifdef NFC_DEMO_DEBUG
    Serial.print("Didn't Find PN53x Module");
    #endif
    while (1); // Halt
  }
  // Configure board to read RFID tags
  nfc.SAMConfig();
  Serial.println("Waiting for an ISO14443A Card ...");
}
  // 清除畫面
 void testdrawstyles(void) {
  display.clearDisplay();
  display.setTextSize(2.5);             // 設定文字大小
  display.setTextColor(1);        // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
  display.setCursor(0,0);             // 設定起始座標
  display.print("CGU  STORE");        // 要顯示的字串
  display.display();                  // 要有這行才會把文字顯示出來
  delay(1000);
}
void test1(void) {
  display.clearDisplay();
  display.setTextSize(2.5);             // 設定文字大小
  display.setTextColor(1);        // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
  display.setCursor(0,0);             // 設定起始座標
  display.print("jason");        // 要顯示的字串
  display.display();                  // 要有這行才會把文字顯示出來
  delay(1000);
}
void test2(void) {
  display.clearDisplay();
  display.setTextSize(2.5);             // 設定文字大小
  display.setTextColor(1);        // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
  display.setCursor(0,0);             // 設定起始座標
  display.print("melody");        // 要顯示的字串
  display.display();                  // 要有這行才會把文字顯示出來
  delay(1000);
}
void test3(void) {
  display.clearDisplay();
  display.setTextSize(2.5);             // 設定文字大小
  display.setTextColor(1);        // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
  display.setCursor(0,0);             // 設定起始座標
  display.print("bond");        // 要顯示的字串
  display.display();                  // 要有這行才會把文字顯示出來
  delay(1000);
}
void init_display(){Serial.println("Hi!! Pass Card!!");}
void loop() {
 uint32_t id=nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A);
  if(id!=0){
    if(id==629612135)
    {  

      test1();
      init_display();
    }
    else if(id==902964327)
    {
      test2();
      init_display();
    }
   else if(id==635568191)
    {
      test3();
      init_display();
    }
    else if(id==23500626759694848)
    {
      test1();
      init_display();
    }
    else{display.clearDisplay();
         display.setTextSize(3);             // 設定文字大小
         display.setTextColor(1);        // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
         display.setCursor(0,0);             // 設定起始座標
         display.print("not member");        // 要顯示的字串
         display.display();                  // 要有這行才會把文字顯示出來
         delay(1000);
         testdrawstyles();}
    }
  }   
