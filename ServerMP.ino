#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

ESP8266WebServer server;

char* ssid = "Your_SSID";
char* password = "Your_PASSWD";
String percent="Checking...";
String stat = "Checking...";
int minp = 20;
int maxp = 80; 
int relayPin=12;
int LEDpin = 16;
boolean override=false;
int flag=0;

void setup() {
  // put your setup code here, to run once:
  SPIFFS.begin();
  WiFi.begin(ssid,password);
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.print(WiFi.localIP());
  pinMode(relayPin,OUTPUT);
  pinMode(LEDpin,OUTPUT);
  server.on("/",serveIndexFile);
  server.on("/update",getData);
  server.on("/override",setOverride);
  server.on("/geto",getOverride);
  server.begin();
  digitalWrite(relayPin,HIGH);
  digitalWrite(LEDpin,LOW);
  stat = "Charging";
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  clearSerial();
  while(!(Serial.available())){
    server.handleClient();
  }
  percent = Serial.readString();
  if(override){
    maxp = 100;
    if(percent.toInt()<maxp){
      digitalWrite(relayPin,HIGH);
      digitalWrite(LEDpin,LOW);
      stat = "Charging";
    }
    else{
      digitalWrite(relayPin, LOW);
      digitalWrite(LEDpin, HIGH);
      stat = "Discharging";
    }
  }
  else{
    maxp = 80;
    if(percent.toInt()>=maxp){
      digitalWrite(relayPin,LOW);
      digitalWrite(LEDpin,HIGH);
      stat = "Discharging";
    }
    else if(percent.toInt()<=minp){
      digitalWrite(relayPin,HIGH);
      digitalWrite(LEDpin,LOW);
      stat = "Charging";
    }
  }
}

void serveIndexFile(){
  File file = SPIFFS.open("/indexp.html","r");
  server.streamFile(file,"text/html");
  file.close();
}

void clearSerial(){
  while(Serial.available()){
    Serial.readString();
  }
}

void getData(){ 
 String res = "{ \"percent\": \""+percent+"\",\"status\": \""+stat+"\"}";
 Serial.println(res);
 server.send(200,"text/plain",res);
}

void setOverride(){
   override = !override;
//  if(override==true){
//    maxp = 100;
//  }
//  else{
//    maxp = 80;
//  }
 
}

void getOverride(){
  String res = "{ \"override\": \""+String(override)+"\"}";
  server.send(200,"text/plain",res);
}
