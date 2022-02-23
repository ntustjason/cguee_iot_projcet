#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>

#define DHTTYPE DHT11
#define DHTPIN  14

// 設定網路基地台SSID跟密碼
const char* ssid      = "iphone";
const char* password  = "00000000";

// 請更換成 Thing Speak WRITE API KEY
const char* apiKey = "PHW2G7KGVOWZ12D9";
const char* resource = "/update?api_key=";

// Thing Speak API server 
const char* server = "api.thingspeak.com";

// 初始化 DHT sensor.
DHT dht(DHTPIN, DHTTYPE); 

float humidity, temp_f;    // 從 DHT-11 讀取的值
static char ftemp[7];

unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor

void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(9600);
  delay(10);

  Serial.println("");
  Serial.println("");
  Serial.print("Connecting To: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // 等待連線，並從 Console顯示 IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // sleeping so wont get here
  // 量測間等待至少 2 秒
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {
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

  Serial.println(temp_f);
  Serial.println(humidity);

  float tempe = temp_f ;
  dtostrf(tempe, 6, 2, ftemp);
  dtostrf(humidity, 6, 2, ftemp);

  // 除錯用
  Serial.println(tempe);
  Serial.println(humidity);
  
  Serial.print("Connecting to "); 
  Serial.print(server);
  WiFiClient client;

  // 使用 80 Port 連線
  if (client.connect(server, 80)) {
    Serial.println(F("connected"));
  } 
  else  {
    Serial.println(F("connection failed"));
    return;
  }
   
  Serial.print("Request resource: "); 
  Serial.println(resource);
  client.print(String("GET ") + resource + apiKey + "&field1=" + tempe + "&field2=" + humidity +
                  " HTTP/1.1\r\n" +
                  "Host: " + server + "\r\n" + 
                  "Connection: close\r\n\r\n");
                  
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  
  if(!client.available()) {
     Serial.println("No response, going back to sleep");
  }
  while(client.available()){
    Serial.write(client.read());
  }
  
  Serial.println("\nclosing connection");
  client.stop();

  // 每三分鐘會上傳一筆資料
  delay(15000);
}
