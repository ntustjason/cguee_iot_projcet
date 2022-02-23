#include <SoftwareSerial.h>
#include <WiFiEsp.h>
#include <PubSubClient.h>
#include <DHT.h>

#define WIFI_SSID "iphone"  //填入WiFi帳號
#define WIFI_PASSWORD "00000000"  //填入WiFi密碼

const char* mqttServer = "172.20.10.4";  //填入MQTT Broker的IP或Domain
const int mqttPort = 1883;  //MQTT Broker的Port

const char* clientID = "arduinoClient1";  //填入自訂ID
const char* topic = "Arduino/DHT22";  //填入自訂主題名稱(階層式命名)

int WiFi_Status = WL_IDLE_STATUS; //ＷiFi狀態

unsigned long prevMillis = 0; //暫存經過時間(單位:毫秒)

SoftwareSerial ESP8266(4,5);  //設定序列埠物件
DHT dht(6,DHT11);  //設定DHT物件

WiFiEspClient espClient;  //設定WiFiEspClient物件
PubSubClient client(espClient); //設定PubSubClient物件(帶入espClient)

void setup() {
  
    //設定序列埠傳輸速率(9600bps)
    Serial.begin(9600);   

    //wifi設定
    wifi_Setting();
  
    //初始化dht
    dht.begin();

    //MQTTServer設定
    client.setServer(mqttServer, mqttPort);
}

void loop() {
  
    if (!client.connected()) {
        Serial.println("MQTT斷線中");     
        reconnectMQTT(); //MQTT重新連線        
    }

    //每兩秒更新數值
    if ( millis() - prevMillis > 2000 ) {       
        getData_sendToMqtt(); //取得與發布數值        
        prevMillis = millis();
    }
  
    client.loop();
}

//wifi設定方法
void wifi_Setting(){
    //設定ESP8266傳輸速率(9600bps)
    ESP8266.begin(9600); 
    
    //初始化ESP模組
    WiFi.init(&ESP8266);

    Serial.print("進行WiFi設定!\r\n");
    do{
        Serial.println("WiFi 連接中 ...");
        WiFi_Status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        delay(500);
    } while (WiFi_Status != WL_CONNECTED);
    
    Serial.print("ＷiFi 連接成功!\r\n");
    Serial.print("IP 位址: ");       
    Serial.println(WiFi.localIP());
    Serial.print("SSID: ");     
    Serial.println(WiFi.SSID());
    Serial.println("WiFi 設定結束\r\n");
}

//MQTT重新連接方法
void reconnectMQTT() {
    while (!client.connected()) {
        if (client.connect(clientID)) {
            Serial.println("MQTT 已連接!");
        } else {
            Serial.print("失敗!, rc=");
            Serial.print(client.state());
            Serial.println("三秒之後重試...");
            
        }
    }
}

//取得與傳送數值
void getData_sendToMqtt(){
    float h = dht.readHumidity(); //讀取模組的濕度資料
    float t = dht.readTemperature(); //讀取模組的溫度資料

    //判斷取得的數值是否是NAN
    if (isnan(h) || isnan(t)) {
        Serial.println("從DHT sensor取得資料失敗!");
        return;
    }

    String temperature = String(t);
    String humidity = String(h);

    //製作JSON格式的payload string
    String payload = "{";
    payload += "\"temperature\":"; 
    payload += temperature; payload += ",";
    payload += "\"humidity\":"; 
    payload += humidity;
    payload += "}";

    //發布payload
    byte arrSize = payload.length() + 1;
    char message[arrSize];
    payload.toCharArray(message,arrSize);
    client.publish(topic,message);
    Serial.println(message);
}
