#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <WiFiEsp.h>
#include <MFRC522.h>  // 引用程式庫
#include <SoftwareSerial.h>
#define RST_PIN      A0        // 讀卡機的重置腳位
#define SS_PIN       10        // 晶片選擇腳位
#define SCREEN_WIDTH 128 // OLED 寬度像素
#define SCREEN_HEIGHT 64 // OLED 高度像素
#define OLED_RESET  4// Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

struct RFIDTag {    // 定義結構
   byte uid[7];
   char *name;
};
struct RFIDTag tags[] = {  // 初始化結構資料
   {{83,117,182,137,2,152,0}, "Arduino"},
   {{83,229,226,169,2,24,0}, "Raspberry Pi"},
   {{83,100,178,163,2,168,0}, "Espruino"}
};
byte totalTags = sizeof(tags) / sizeof(RFIDTag);
MFRC522 mfrc522(SS_PIN, RST_PIN);  // 建立MFRC522物件
void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 一般1306 OLED的位址都是0x3C
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);} 
  display.display();
  delay(1000); 
  display.clearDisplay();
  testdrawstyles();
  Serial.begin(115200);
  Serial.println();
  Serial.println("RFID reader is ready!");
  SPI.begin();
  mfrc522.PCD_Init(); // 初始化MFRC522讀卡機模組
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
    // 確認是否有新卡片
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      byte *id = mfrc522.uid.uidByte;   // 取得卡片的UID
      byte idSize = mfrc522.uid.size;   // 取得UID的長度
      bool foundTag = false;            // 是否找到紀錄中的標籤，預設為「否」。     
      for (byte i=0; i<totalTags; i++) {
        if (memcmp(tags[i].uid, id, idSize) == 0) {
          Serial.println(tags[i].name);  // 顯示標籤的名稱
         display.clearDisplay();
  display.setTextSize(4);             // 設定文字大小
  display.setTextColor(1);        // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
  display.setCursor(0,0);             // 設定起始座標
  display.println(tags[i].name);        // 要顯示的字串
  display.display();                  // 要有這行才會把文字顯示出來
  delay(1000);
          foundTag = true;  // 設定成「找到標籤了！」
          break;            // 退出for迴圈
        }
      }
      if (!foundTag) {    // 若掃描到紀錄之外的標籤，則顯示"Wrong card!"。
        Serial.println("Wrong card!");
      }
  mfrc522.PICC_HaltA();
    } 
}
