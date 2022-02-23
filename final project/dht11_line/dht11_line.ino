
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <TridentTD_LineNotify.h>
#define DHTTYPE DHT11
#define DHTPIN  14

// 修改成上述寄到登入郵箱的 Token號碼
#define LINE_TOKEN "XbV1bFbYmcy2RUhBhwQP3adLzJbtsyzAkJ0bYXCHROT"

// 設定無線基地台SSID跟密碼
const char* ssid     = "iphone";
const char* password = "00000000";

DHT dht(DHTPIN, DHTTYPE, 11);    // 11 works fine for ESP8266
float humidity, temp_f;   // 從 DHT-11 讀取的值
unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor

// 用不到以下兩個變數
// const char* host = "notify-api.line.me";
// const int httpsPort = 443;

void setup(void)
{
  Serial.begin(9600);  // 設定速率 感測器
  dht.begin();           // 初始化
  WiFi.mode(WIFI_STA);
  // 連接無線基地台
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect");
  // 等待連線，並從 Console顯示 IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("DHT Weather Reading Server");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop(void)
{
  // 量測間等待至少 2 秒
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // 將最後讀取感測值的時間紀錄下來
    previousMillis = currentMillis;
    // 讀取溫度大約 250 微秒!
    humidity = dht.readHumidity();          // 讀取濕度(百分比)
    temp_f = dht.readTemperature();     // 讀取溫度(華氏)
    // 檢查兩個值是否為空值
    if (isnan(humidity) || isnan(temp_f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  }
 if ((int)temp_f >= 27 || (int)humidity >= 80) {
    //組成Line訊息內容
    String message = "檢測環境發生異常，請協助儘速派人查看處理，目前環境狀態：";
  String tempe = "溫度:" + String((int)(temp_f )) + "℃";
  String humid = "濕度:" + String((int)humidity) + "％";
 

  // 顯示 Line版本
  Serial.println(LINE.getVersion());
  LINE.setToken(LINE_TOKEN);
  // 先換行再顯示
 LINE.notify("\n"+message);
  LINE.notify("\n"+ tempe + " ；" + humid);
  // 每2分鐘發送一次
 
}
}
