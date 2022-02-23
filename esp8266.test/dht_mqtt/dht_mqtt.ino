#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#define DHTPIN 12  //讀取DHT11 Data
#define DHTTYPE DHT11 //選用DHT11   
#define humidity_topic "sensor/humidity"
#define temperature_topic "sensor/temperature"
const char* ssid="iphone";
const char* password="00000000";
const char* mqtt_server="172.20.10.4";
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN,DHTTYPE);
void setup(){
  Serial.begin(9600);
  Serial.print("DHT TEST!");
  setup_wifi();
  client.setServer(mqtt_server,1883);
  dht.begin();
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
  Serial.println(WiFi.localIP());}
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
  long lastMsg = 0;
  float temp =0.0;
  float hum =0.0;
  float diff =1.0;
  void loop(){
    if(!client.connected()){reconnect();}
    client.loop();
    long now=millis();
    if (now - lastMsg >5000){lastMsg=now;
    float newHum=dht.readHumidity();
    float newTemp=dht.readTemperature();
    Serial.print("new temperature");
    Serial.println(newTemp);
    client.publish(temperature_topic, String(newTemp).c_str(), true);
    Serial.print("new humidity:");
    Serial.println(newHum);
    client.publish(humidity_topic, String(newHum).c_str(), true);
  }
  

  }
