#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

//          format request
//          /khoi/request            lay command tiep theo
//          /khoi/done                bao xong roi
//          /cuong/request           lay command tiep theo
//          /cuong/done               bao xong roi

const char* ssid = "TellMeNow2";
const char* password = "notsogoodisntit";

const int port = 80;
const int port1 = 90;
const int port2 = 100;


int khoiCounter = 0;
int cuongCounter = 0;

bool kd = false;
bool cd = false;

String khoicommand[2] = {"auto0","auto0"};
String cuongcommand[2] = {"auto0","auto0"};

WiFiServer server(port);
ESP8266WebServer server1(port1);
ESP8266WebServer server2(port2);

void khoiRequest() {
  server1.send(200, "text/plain", khoicommand[khoiCounter].c_str());
  if(++khoiCounter == sizeof(khoicommand)/sizeof(khoicommand[0]) ){
    khoiCounter = 0; 
	khoicommand[1] = "auto0";
  }
  
}
void cuongRequest() {
   server2.send(200, "text/plain", cuongcommand[cuongCounter].c_str());
  if(++cuongCounter == sizeof(cuongcommand)/sizeof(cuongcommand[0]) ){
    cuongCounter = 0; 
    cuongcommand[1] = "auto0";
  }
}

void khoiDone(){
  server1.send(200, "text/plain", "ok");
  kd = true;
}
void cuongDone(){
  server1.send(200, "text/plain", "ok");
  kd = true;
}


void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  server1.on("/khoi/done", khoiRequest);
  server1.begin();
  
  server2.on("/cuong/done", cuongRequest);
  server2.begin();

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server1.handleClient();
  server2.handleClient();
  WiFiClient client = server.available();
  if (!client) 
  { 
    return; 
  } 
  Serial.println("Somebody has connected :)");  
  String request = client.readStringUntil('\r');
  Serial.println(request);
  
  // Handle the Request
  if (request.indexOf("/MANUAL") != -1)
  {
    Serial.println("MANUAL");
    khoicommand[1] = "manu1";
    cuongcommand[1] = "manu1";
  }

  if (request.indexOf("COLAB") != -1)
  {
    Serial.println("COLAB");
    khoicommand[1] = "auto4";
    cuongcommand[1] = "auto4";
  }

  else if (request.indexOf("/KHOI_PLAY_1") != -1)
  {
    Serial.println("Khoi Song 1");
    khoicommand[1] = "auto1";

  }
   else if (request.indexOf("/KHOI_PLAY_2") != -1)
  {
    Serial.println("Khoi Song 2");
    khoicommand[1] = "auto2";

  }
   else if (request.indexOf("/KHOI_PLAY_3") != -1)
  {
    Serial.println("Khoi Song 3");
    khoicommand[1] = "auto3";

  }
   else if (request.indexOf("/CUONG_PLAY_1") != -1)
  {
    Serial.println("Cuong Song 1");
    cuongcommand[1] = "auto1";

  }

  else if (request.indexOf("/CUONG_PLAY_2") != -1)
  {
    Serial.println("Cuong Song 2");
    cuongcommand[1] = "auto2";

  }

  else if (request.indexOf("/CUONG_PLAY_3") != -1)
  {
    Serial.println("Cuong Song 3");
    cuongcommand[1] = "auto3";

  }
  else if (request.indexOf("/STOP") != -1)
  {
    Serial.println("Stop");
    khoicommand[1] ="auto0";
    cuongcommand[1]="auto0";

  }
  
  // Prepare the HTML document to respond and add buttons:
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  
  s += "<br><input type=\"button\" name=\"b1\" value=\"Manual\"";
  s += " onclick=\"location.href='/MANUAL'\">";
  s += "<br><br><br>";

  s += "<br><input type=\"button\" name=\"b1\" value=\"Khoi_play_1\"";
  s += " onclick=\"location.href='/KHOI_PLAY_1'\">";
  s += "<br><br><br>";

  s += "<br><input type=\"button\" name=\"b1\" value=\"Khoi_play_2\"";
  s += " onclick=\"location.href='/KHOI_PLAY_2'\">";
  s += "<br><br><br>";

  s += "<br><input type=\"button\" name=\"b1\" value=\"Khoi_play_3\"";
  s += " onclick=\"location.href='/KHOI_PLAY_3'\">";
  s += "<br><br><br>";

  s += "<br><input type=\"button\" name=\"b1\" value=\"Cuong_play_1\"";
  s += " onclick=\"location.href='/CUONG_PLAY_1'\">";
  s += "<br><br><br>";

  s += "<br><input type=\"button\" name=\"b1\" value=\"Cuong_play_2\"";
  s += " onclick=\"location.href='/CUONG_PLAY_2'\">";
  s += "<br><br><br>";

  s += "<br><input type=\"button\" name=\"b1\" value=\"Cuong_play_3\"";
  s += " onclick=\"location.href='/CUONG_PLAY_3'\">";
  s += "<br><br><br>";

   s += "<br><input type=\"button\" name=\"b1\" value=\"Colab\"";
  s += " onclick=\"location.href='/COLAB'\">";
  s += "<br><br><br>";
  
  s += "<br><input type=\"button\" name=\"b1\" value=\"Stop\"";
  s += " onclick=\"location.href='/STOP'\">";
  s += "<br><br><br>";

  
  
  s += "</html>\n";
  
  //Serve the HTML document to the browser.
  client.flush(); //clear previous info in the stream
  client.print(s); // Send the response to the client
  delay(1);
  Serial.println("Client disonnected"); //Looking under the hood
}
