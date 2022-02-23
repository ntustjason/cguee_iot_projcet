
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <DHT.h>
#include <MySQL_Encrypt_Sha1.h>
#include <TridentTD_LineNotify.h>
#include <MySQL_Packet.h>
#define LINE_TOKEN "XbV1bFbYmcy2RUhBhwQP3adLzJbtsyzAkJ0bYXCHROT"
#define PIR_PIN 12 //感測器腳位
#define LED_PIN 4 //LED燈腳位
#define DHTPIN  13
#define DHTTYPE DHT11
const char* ssid    = "iphone";// change to your WIFI SSID
const char* password = "00000000";// change to your WIFI Password
IPAddress server_addr(172,20,10,4); // change to you server ip, note its form split by "," not "."
int MYSQLPort = 3306; //mysql port default is 3306
char user[] = "iot1";// Your MySQL user login username(default is root),and note to change MYSQL user root can access from local to internet(%)
char pass[] = "a458jason";// Your MYSQL password
int k=0;
bool pirState = false;
WiFiClient espclient;
MySQL_Connection conn((Client *)&espclient);
DHT dht(DHTPIN, DHTTYPE, 11);
float humidity, temp_f; 
unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor
void setup() {
  Serial.begin(115200);
  dht.begin(); 
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  //try to connect to WIFI
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //try to connect to mysql server
  if (conn.connect(server_addr, 3306, user, pass)) {
    delay(1000);
  }
  else {
    Serial.println("Connection failed.");
  }
  delay(2000);

}

void loop(void) {
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
  Serial.print((int)temp_f); Serial.print(" *C, ");
  Serial.print((int)humidity); Serial.println(" H");
 if ((int)temp_f >= 29 || (int)humidity >= 80) {
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
  }

    pirState = digitalRead(PIR_PIN); 
    if (pirState== true ) {
    k=k+1;
    digitalWrite(LED_PIN, HIGH);
    // 模块感应到人.输出高电平.点亮LED
    Serial.print("pass");
    Serial.print("\n");
    delay(800);
  }
  else {
    digitalWrite(LED_PIN, LOW);   //  无人状态保持LED关闭
    Serial.print("no one"); 
    Serial.print("\n");
    delay(800);
  }
  String INSERT_SQL =  "INSERT INTO temphumid.new_table (temp,humid) VALUES ('" + String((int)temp_f ) + "' , '" + String((int)humidity) + "')";   "INSERT INTO detect.human (total) VALUES ('" + String((int)k) + "')";
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(INSERT_SQL.c_str());//execute SQL
  delete cur_mem;
  Serial.println("Data Saved.");
}
