#include <ESP8266WiFi.h>
#include <Wire.h>
#include <SPI.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// WiFi parameters
#define WLAN_SSID       "iphone"
#define WLAN_PASS       "00000000"
// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "iot_jason"
#define AIO_KEY         "aio_NMBB38Y9EIp6oGPyt5SQoY3Pa2oY"  // Obtained from account info on io.adafruit.com
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET 4     // Reset pin # (or -1 if sharing Arduino reset pin)
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
 
Adafruit_MQTT_Publish Attendance = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/RFID_ITEM.DETECT");

char ID;
/*************************** Sketch Code ************************************/
 
void setup() { 

  Serial.begin(115200);   
  Wire.begin(0, 2);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //turning on the oled
  display.clearDisplay();
  testdrawstyles();

  Serial.println(F("Adafruit IO Example"));
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Connecting to "));
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
 
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
 
  // connect to adafruit io
  connect();
 
}
 void testdrawstyles(void) {
  display.clearDisplay();
  display.setTextSize(2.5);             // 設定文字大小
  display.setTextColor(1);        // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
  display.setCursor(0,0);             // 設定起始座標
  display.print("IOT  STORE");        // 要顯示的字串
  display.display();                  // 要有這行才會把文字顯示出來
  delay(1000);
}
// connect to adafruit io via MQTT
void connect() {
  Serial.print(F("Connecting to Adafruit IO... "));
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }
 
    if(ret >= 0)
      mqtt.disconnect();
 
    Serial.println(F("Retrying connection..."));
    delay(5000);
  }
  Serial.println(F("Adafruit IO Connected!"));
}
 
void loop() {
  // ping adafruit io a few times to make sure we remain connected
  if(! mqtt.ping(3)) {
    // reconnect to adafruit io
    if(! mqtt.connected())
      connect();
  }
   if ( Serial.available() ) { // Update and send only after 1 seconds
   char a = Serial.read();
   ID = a;  
   if(ID==1){
   display.clearDisplay();
   display.setCursor(0, 0);
    display.println("cookie $10"); 
    display.display();
    }
    if(ID==2){
   display.clearDisplay();
   display.setCursor(0, 0);
    display.println("CHOCALATE $20"); 
    display.display();
    }
    if(ID==3){
   display.clearDisplay();
   display.setCursor(0, 0);
    display.println("cake $30"); 
    display.display();
    }
    if(ID==4){
   display.clearDisplay();
   display.setCursor(0, 0);
    display.println("candy $40"); 
    display.display();
    }
    if(ID==5){
   display.clearDisplay();
   display.setCursor(0, 0);
    display.println("salad $50"); 
    display.display();
    }
    if(ID==6){
   display.clearDisplay();
   display.setCursor(0, 0);
    display.println("BEER $60"); 
    display.display();
    }
   if (! Attendance.publish(ID)) {                     //Publish to Adafruit
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("Sent!"));
    }
}
}
