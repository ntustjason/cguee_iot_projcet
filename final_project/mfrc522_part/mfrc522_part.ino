#include <MFRC522.h>
#include "SoftwareSerial.h"
#define SS_PIN 10
#define RST_PIN A0
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
SoftwareSerial ser(2,3); // RX, TX 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  ser.begin (115200);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Put RFID Card to Scan...");
  Serial.println();
 
}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
    mfrc522.PICC_HaltA();
  }
  Serial.println();
  Serial.print("User No./Name:    ");
  content.toUpperCase();
  if (content.substring(1) == "53 FD A2 A9 02 97 00" ) //change here the UID of the card/cards that you want to give access
  {
    Serial.println("COOKIE-$10");
    ser.write(1);
    Serial.println();
    
    delay(1000);
  }
   if (content.substring(1) ==  "53 EC E7 81 02 88 00" ) //change here the UID of the card/cards that you want to give access
  {
    Serial.println("CHOCALATE-$20");
    ser.write(2);
    Serial.println();
    
    delay(1000);
  }
   if (content.substring(1) == "53 3C B3 89 02 67 00" ) //change here the UID of the card/cards that you want to give access
  {
    Serial.println("cake-$30");
    ser.write(3);
    Serial.println();
    
    delay(1000);
  }
   if (content.substring(1) == "BURGER-$40" ) //change here the UID of the card/cards that you want to give access
  {
    Serial.println("4-Simon Jones");
    ser.write(4);
    Serial.println();
    
    delay(1000);
  }
   if (content.substring(1) == "53 B5 B7 A9 02 18 00" ) //change here the UID of the card/cards that you want to give access
  {
    Serial.println("salad-50");
    ser.write(5);
    Serial.println();
    
    delay(1000);
  }
    if (content.substring(1) ==  "53 64 A7 A1 02 98 00" ) //change here the UID of the card/cards that you want to give access
  {
    Serial.println("BEER-$60");
    ser.write(6);
    Serial.println();
    
    delay(1000);
  }
  }
