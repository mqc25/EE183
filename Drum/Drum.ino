#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>

Servo myservo;
Servo myservo3;
int song11[]={1,1,1,1,1,1,1,1,1,3,4}; //#of time to hit the drum at current pos
int song12[]={1,2,3,1,2,3,3,2,1,1,3};//1=120, 2 = 90, 3 = 45

int song21[]={4,1,3,1,2,1};
int song22[]={2,2,1,2,1,2};

int song31[]={2,1,2,1,2,1,2,1,2,1};
int song32[]={2,3,2,3,2,3,2,3,2,3};

const char* ssid = "TellMeNow2";
const char* password = "notsogoodisntit";

const char* host = "http://192.168.4.1";
String msg = "ok";
bool manualFlag = false;
             
int httpCode;
int port = 100;
IPAddress server(192, 168, 4, 1);
char* ip = "192.168.4.1";
HTTPClient http;

void rptaSrv(int httpCode) {
  msg = "";
  if (httpCode == 200) {
    int len = http.getSize();

    // create buffer for read
    char buff[128];

    // get tcp stream
    WiFiClient * stream = http.getStreamPtr();

    // read all data from server
    while (http.connected() && (len > 0 || len == -1)) {
      // get available data size
      size_t size = stream->available();

      if (size) {
        // read up to 128 byte
        int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

        // write it to Serial
        for (int j = 0; j < c; j++) {
          msg.concat(buff[j]);
        }
        if (len > 0) {
          len -= c;
        }
      }
    }
  }
}

void getMsg() {
  //http.begin(ip, port, s); // 3rd argument request format
 
  httpCode = http.GET();
  yield();
  rptaSrv(httpCode);
  //http.end();
}

void start()
{
  Serial.println("STARTED");
  
  myservo.attach(D1);  // attaches the servo on GIO1 to the servo object 
  myservo.write(0);   //  turn the servo back to 0 degree
  delay(0);

  myservo3.attach(D4);
  myservo3.write(90);
  delay(0); 
  }

void playsong() // play preloaded pattern
{
  for(int i = 0; i < sizeof(song11)/sizeof(song11[0]); i++)
    {
      play(song11[i], song12[i]);
       delay(30);
    }
    Serial.println("DONE song 1");
    stopservo();
 }

 void playsong2()
{
  for(int i = 0; i < 6; i++)
    {
      play(song21[i], song22[i]);
       delay(30);
    }
    Serial.println("DONE song2");
    stopservo();
 }

 void playsong3()
{
  for(int i = 0; i < sizeof(song31)/sizeof(song31[0]); i++)
    {
      play(song31[i], song32[i]);
       delay(30);
    }
    Serial.println("DONE song3");
    stopservo();
 }
  
void stopservo() // stop the servo to save battery
{
 myservo.write(0);
  delay(500);
  myservo.detach();
  
  myservo3.write(0);
  delay(500);
  myservo3.detach();
 }

void hitthedrum() //command the servo to turn 30 degrees
{
  myservo.write(30);
  }
void resetpos() ////command the servo to turn back to 0 degree
{
  myservo.write(0); 
  }

 void play(int s1, int s2) // combine 2 preloaded patterns of 2 servos
{
  if (s2 == 1)
  {
    myservo3.write(115);
    delay(400);
    } 
  else if(s2 == 2) 
  {
    myservo3.write(80);
    delay(400);
    }
  else if (s2 == 3)
  {
    myservo3.write(55);
    delay(500);
    }  
  
  
  for (int i = 0; i < s1; i++)
  {
    hitthedrum();
    delay(300);
    resetpos();
    delay(300);
    }
 } 

 void Manual() // can turn the servo using the touch sensor (no preloaded patterns needed)

  {
      start();
    for(int i = 0; i < 1000; i++){
      if(digitalRead(D5) == HIGH)
      {
       hitthedrum();
      }
      else
      {
        resetpos();
       }
       delay(10);
    } 

    Serial.println("You ran out of time!");
    stopservo();
  }

  void collab() // used for collaborating with another robot
  { 
    start();
   playsong3();
   double time_l = millis();
   while (true){
       if(digitalRead(D5) == HIGH){
       break;
      }
      delay(50);
   }
   start();
   playsong();
  }
void setup()
{
  //Setup wifi connection and connect to the server to get command
  Serial.begin(115200);
  Serial.println();
  delay(1000);
  Serial.printf("Connecting to %s ", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(D5, INPUT); //Add touch sensor as input
}

String temp="";

//decode the command from server and do the job respectively
void decide(bool manFlag, String msg){
  if(manFlag){
    Manual();
  }
  else if(msg=="1"){
    start();
    playsong();
    
  }
  else if(msg=="2"){
    start();
    playsong2();
  }
  else if(msg=="3"){
    start();
    playsong3();
  }
  else if(msg=="4"){
    collab();
  }
}

void loop() {


  WiFiClient client;
  while (client.connect(server, port) != 1)
  {
    Serial.println("connection failed! try again!");
    client.stop();
  }
  http.begin(ip, port, "/cuong/done");
  while (true) {
    do {
      getMsg();
      temp = msg.substring(0,4);
    }
    while (temp != "manu" && temp != "auto" ) ;
    
    if (temp == "manu") manualFlag = true;
    else manualFlag = false;

    msg.remove(0, 4);
    Serial.println(msg);
    decide(manualFlag,msg);
    delay(250);
    // act according to flag
    // if manual act do single action depend on argument
    // if auto do predefine sequence of action depend on argument
    // function take in two arg(bool manualFlag, int action)
    Serial.println("loop");
  }

}

