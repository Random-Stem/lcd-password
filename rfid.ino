#include <SPI.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>
MFRC522 mfrc522(10,9);
String a="";
void setup() {
Serial.begin(9600);
SPI.begin();
mfrc522.PCD_Init();
pinMode(8,OUTPUT);
}

void loop() {
if(! mfrc522.PICC_IsNewCardPresent()){
  return;
}
if(! mfrc522.PICC_ReadCardSerial()){
  return;
}
for(byte i=0;i<mfrc522.uid.size;i++){
  if(mfrc522.uid.uidByte[i] < 0x10 ){
   a +=0;
  }
  a +=String(mfrc522.uid.uidByte[i],HEX);
}
Serial.println(a);
mfrc522.PICC_HaltA();
if(a){
  Serial.println(a);
if(a=="730522a6"){
  digitalWrite(8,HIGH);
  delay(300);
  digitalWrite(8,LOW);
  Serial.println("si");
  a="";
}else{
  Serial.println("no");
  digitalWrite(7,HIGH);
  delay(300);
  digitalWrite(7,LOW);
  a="";
}
}
}