#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <MySQL_Packet.h>
#include <TridentTD_LineNotify.h>
#define PIR_PIN 12 //感測器腳位
#define LED_PIN 4 //LED燈腳位
#define DHTPIN 13  //讀取DHT11 Data
#define DHTTYPE DHT11 //選用DHT11   
#define humidity_topic "sensor/humidity"
#define temperature_topic "sensor/temperature"
#define human_topic "sensor/human"
#define LINE_TOKEN "XbV1bFbYmcy2RUhBhwQP3adLzJbtsyzAkJ0bYXCHROT"
const char* ssid="iphone";
const char* password="00000000";
const char* mqtt_server="172.20.10.4";
IPAddress server_addr(172,20,10,4); // change to you server ip, note its form split by "," not "."
int MYSQLPort = 3306; //mysql port default is 3306
char user[] = "iot1";// Your MySQL user login username(default is root),and note to change MYSQL user root can access from local to internet(%)
char pass[] = "a458jason";// Your MYSQL password
WiFiClient espClient,client1;
PubSubClient client(espClient);
MySQL_Connection conn((Client *)& client1);
DHT dht(DHTPIN,DHTTYPE);
void setup(){
  Serial.begin(9600);
  Serial.print("iot TEST!");
  pinMode(PIR_PIN,INPUT);//設定人體感測器腳位為輸入
  pinMode(LED_PIN,OUTPUT);//設定LED燈腳位為輸出
  setup_wifi();
  client.setServer(mqtt_server,1883);
  dht.begin();
   if (conn.connect(server_addr, 3306, user, pass)) {
    delay(1000);
  }
  else {
    Serial.println("Connection failed.");
  }
  delay(2000);
}

void setup_wifi(){
  delay(10);
  Serial.println();
  Serial.print("connecting to");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED){delay(500);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connnected");
  Serial.println("ip address:");
  Serial.println(WiFi.localIP());
  }
  void reconnect(){
    while(!client.connected()){
      Serial.print("Attempting mqtt connection");
      if(client.connect("mqtt_client")){
      Serial.println("   .....connected");
    } else{
      Serial.print("mqtt connection failed,retry count:");
      Serial.print(client.state());
      Serial.println("try again in 5second");
      delay(5000);
      }
  }
  }
  int k=0;
  long lastMsg = 0;
  float temp =0.0;
  float hum =0.0;
  float diff =1.0;
  void loop(){
    if(!client.connected()){reconnect();}
    client.loop();
     long now=millis();
     bool pirState = false;
     //建立变量存储感应模块的输出信号
     pirState = digitalRead(PIR_PIN); 
    if (pirState== true ) {
    k=k+1;
    digitalWrite(LED_PIN, HIGH);
    // 模块感应到人.输出高电平.点亮LED
    Serial.print("pass");
    delay(2000);
    client.publish(human_topic, String(k).c_str(), true);
    Serial.print("\n");
     String INSERT_SQL1 ="INSERT INTO detect.human (total) VALUES ('" + String((int)k) + "')";  
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(INSERT_SQL1.c_str());//execute SQL
  delete cur_mem;
  Serial.println("Data Saved.");
  }
  else {
    
    digitalWrite(LED_PIN, LOW);   //  无人状态保持LED关闭
    Serial.print("no one");
    Serial.print("\n"); 

  }
    if (now - lastMsg >10000){
     lastMsg=now;
    float newHum=dht.readHumidity();
    float newTemp=dht.readTemperature();
    Serial.print("new temperature");
    Serial.println(newTemp);
    client.publish(temperature_topic, String(newTemp).c_str(), true);
    Serial.print("new humidity:");
    Serial.println(newHum);
    client.publish(humidity_topic, String(newHum).c_str(), true);
    if ((float)newTemp >= 31|| (float)newHum >= 80) {
    //組成Line訊息內容
    String message = "檢測環境發生異常，請協助儘速派人查看處理，目前環境狀態：";
    String tempe = "溫度:" + String((float)newTemp) + "℃";
    String humid = "濕度:" + String((float)newHum) + "％";
  // 顯示 Line版本
  Serial.println(LINE.getVersion());
  LINE.setToken(LINE_TOKEN);
  // 先換行再顯示
  LINE.notify("\n"+message);
  LINE.notify("\n"+ tempe + " ；" + humid);
  // 每2分鐘發送一次
 }
  String INSERT_SQL ="INSERT INTO temphumid.new_table (temp,humid) VALUES ('" + String((int)newTemp) + "','" + String((int)newHum) + "')";  
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(INSERT_SQL.c_str());//execute SQL
  delete cur_mem;
  Serial.println("Data Saved.");
  
  }}
