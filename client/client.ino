#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "EE183DA-1";
const char* password = "Nuthead";
const char terminate = '\n';
const int port = 99;
String ack = "0\n";
String msg ="";
WiFiClient client;

int getMsg(){
  
  if (client.available()) {
        msg = client.readStringUntil(terminate);
        client.flush();
        return 1;
  }
  return 0;
}

void sendMsg(String msg){
  client.print(msg + ack);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  delay(1000);
  WiFi.begin(ssid,password);
  Serial.println("Connecting to local network ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" done");
  if (!client.connect(WiFi.localIP(), port))
    Serial.println("Not connect to local IP");
    client.print(String(port) + "\n");
}

void loop() {
  if (!client.connected())
    Serial.println("Disconnected");
    
  // put your main code here, to run repeatedly:
  
}
