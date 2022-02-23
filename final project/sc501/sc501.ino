#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <SimpleDHT.h>
#include <MySQL_Encrypt_Sha1.h>
#include <MySQL_Packet.h>
const char* ssid    = "iphone";// change to your WIFI SSID
const char* password = "00000000";// change to your WIFI Password
IPAddress server_addr(172,20,10,4); // change to you server ip, note its form split by "," not "."
int MYSQLPort = 3306; //mysql port default is 3306
char user[] = "iot1";// Your MySQL user login username(default is root),and note to change MYSQL user root can access from local to internet(%)
char pass[] = "a458jason";// Your MYSQL password
WiFiClient client;
MySQL_Connection conn((Client *)&client);
int irSensor = 12;
void setup() {
  Serial.begin(115200);
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
  delay(1000);
  // 初始化 串口通讯
  
  pinMode(LED_BUILTIN, OUTPUT); //连接内置LED的引脚设置为输出模式
  pinMode(irSensor, INPUT);     //连接人体红外感应模块的OUT引脚设置为输入模式
}
  
// 
void loop() {
  int k=0;
  bool sensorReading = digitalRead(irSensor);  //建立变量存储感应模块的输出信号
  if ( sensorReading ) {
    k=k+1;
    digitalWrite(LED_BUILTIN, HIGH);
    // 模块感应到人.输出高电平.点亮LED
    Serial.print("pass");
    Serial.print("\n");
    delay(2400);
    
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);   //  无人状态保持LED关闭
 Serial.print("no one");
    Serial.print("\n"); delay(2400);}
    
//将模块输出信号通过串口监视器显示
String INSERT_SQL = "INSERT INTO detect.human (total) VALUES ('" + String((int)k) + "')";
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(INSERT_SQL.c_str());//execute SQL
  delete cur_mem;
  Serial.println("Data Saved.");

  }
