#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <MySQL_Encrypt_Sha1.h>
#include <MySQL_Packet.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
const char* ssid      = "iphone";
const char* password  = "00000000";// change to your WIFI Password
IPAddress server_addr(172,20,10,4);
int MYSQLPort =3306;   //mysql port default is 3306
char user[] = "iot1";// Your MySQL user login username(default is root),and note to change MYSQL user root can access from local to internet(%)
char pass[] = "a458jason";// Your MYSQL password
WiFiClient client;            
MySQL_Connection conn((Client *)&client);
void setup() {
  Serial.begin(115200);
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  //try to connect to WIFI 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("");
  Serial.print("WiFi connected");  
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());
  //try to connect to mysql server
   if (conn.connect(server_addr, 3306, user, pass)) {
     delay(1000);
  }
  else{
    Serial.println("Connection failed.");
  }
  delay(2000);  
  //insert, change database name and values by string and char[]
  char INSERT_SQL[] = "INSERT INTO temphumid.new_table (temp,humid) VALUES ('35','60')";//傳入的值固定為溫度,濕度為35,60
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);  
  cur_mem->execute(INSERT_SQL);//execute SQL
  delete cur_mem;
                 // close the connection
  Serial.println("Data Saved.");
}
void loop() {
//do nothing
}
