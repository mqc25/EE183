#include <ESP8266WiFi.h>

const char* ssid = "EE183DA-1";
const char* password = "Nuthead";
const char terminate = '\n';
const int port1 = 99;
const int port2 = 100;
String ack = "0\n";

WiFiServer server1(port1);
WiFiServer server2(port2);

WiFiClient client1;
WiFiClient client2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println(WiFi.softAP(ssid,password) ? "Ready" : "Failed!");
  server1.begin();
  server2.begin();
  
  client1 = server1.available();
  while(getMsg(client1)!= String(port1)){
    Serial.println("Port 1 not connected");
    delay(250);
  }
  client2 = server2.available();
   while(getMsg(client2)!= String(port2)){
    Serial.println("Port 2 not connected");
    delay(250);
  }
  Serial.println("All port connected");
}

String getMsg(WiFiClient & server){
   String msg = "";
  if(server.available()){
      msg = server.readStringUntil(terminate);
      server.flush();
  }
  return msg;
}

void sendMsg(WiFiClient & server, String message){
  server.print(message + ack);
}
void loop() {
  // put your main code here, to run repeatedly:
  
}
