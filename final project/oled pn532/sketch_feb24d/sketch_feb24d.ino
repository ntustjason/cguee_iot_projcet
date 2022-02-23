#include <PN532.h>
#include <SPI.h>
#include <Wire.h>
#define SCK 13
#define MOSI 11
#define SS 10
#define MISO 12
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LiquidCrystal.h>
#define SCREEN_WIDTH 128 // OLED 寬度像素
#define SCREEN_HEIGHT 64 // OLED 高度像素
// 設定OLED
#define OLED_RESET 4
#define PN532_CS 10
PN532 nfc(PN532_CS);
#define  NFC_DEMO_DEBUG 1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//自訂圖形，從Image2LCD轉過來的，尺寸是128x48，少掉的16個像素是上方黃色區塊(假設是用藍黃雙色的OLED)
//本範例為了在上方區塊顯示文字，所以圖形就沒有涵蓋整個高度
void setup() {
 Serial.begin(115200);
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 一般1306 OLED的位址都是0x3C
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // 顯示Adafruit的LOGO，算是開機畫面
  display.display();
  delay(1000); // 停1秒
  // 清除畫面
  display.clearDisplay();

  testdrawstyles();    // 測試文字
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
void init_display(){
  // show time once in a while
  
  Serial.println("Hi!! Pass Card!!");
  
}
void loop() {
}

void testdrawstyles(void) {
  display.clearDisplay();
  display.setTextSize(2);             // 設定文字大小
  display.setTextColor(1);        // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
  display.setCursor(0,0);             // 設定起始座標
  display.print("CGU  STORE");        // 要顯示的字串
  display.display();                  // 要有這行才會把文字顯示出來
  delay(1000);
}
