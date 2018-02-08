/*Author: Khoi Luc
 *EE 183DA
 *Description: This source code gives the second robot basic movements:
 *  turn right, turn left, turn to the original
I confirmed that all of the codes are all my original work 
*/
#include <Arduino.h>
#include <Servo.h>
#include <stdlib.h>
#include <EEPROM.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "mpu_config.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "TellMeNow2";
const char* password = "notsogoodisntit";

const char* host = "http://192.168.4.1";
String msg = "ok";

Servo base;
Servo left;
Servo right;
double *a;
double temp_x;
double temp_y;
double temp_z;
bool manualFlag = false;


int httpCode;
int port = 90;
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

void manualControl() {
  //future implementation
}

//the left servo hits the piano
void left_hit ()
{
  left.write(10);
  delay(500);
  left.write(80);
  delay(500);
  left.write(10);

}

//the right servo hits the piano
void right_hit()
{
  right.write(120);
  delay(500);
  right.write(40);
  delay(500);
  right.write(120);
  
  
}

//the base return to the original position
void base_return ()
{
  base.write(90);
  
}

//the base servo turns to the right
void base_right()
{
  base.write(80);
}

//the left servo turns to the left
void base_left()
{
  base.write(110);
}



void orin_return()
{
 // base.write(0);
  delay(500);
  left.write(0);
  delay(500);
  right.write(0);
}


// the chord of all the song
void song_1()
{
  left_hit();
  left_hit();
  left_hit();
  right_hit();
  right_hit();
  right_hit();

  
}

void song_2()
{
  left_hit();
  base_left();
  
  left_hit();
  
  right_hit();
  base_return();
  left_hit();
  right_hit();
}


void song_3()
{
  base.write(130);
  left_hit();
  left_hit();
  left_hit();
  left_hit();
  left_hit();
  left_hit();
  left_hit();
  left_hit();
  left_hit();
  left_hit();
  left_hit();
  left_hit();  

}


//take the reading from the gyro
void gyro_read()
{
    a = getMPUReading_acc();
   //Serial.println(a[0]);
    //Serial.println(a[1]);
    delay(100);
   // Serial.println(a[2]);
    
  
}


//below are the code for manual control
void left_turn()
{
  base.write (base.read()+5);
}
void right_turn()
{
  base.write(base.read()-5);
}


void left_up()
{
  left.write(left.read()-5);
}
void left_down()
{
  left.write(left.read()+5);
}
void manual()
{
  gyro_read();
  if( a[0] < -500)
    left_down();
  else if (a[0] >500)
    left_up();
  if (a[1] > 1500)
    left_turn();
  else if (a[1] <-1500)
    right_turn();
}


//assume wait for 10s
bool detect()
{
  bool detect = false;
  double time_1 = millis();
  double track = millis();
 
  while (true) // 10s
  {
    Serial.println("error2");
    
    delay (2000);
    gyro_read();
    
    if ( abs(temp_x - a[0]) <100)
      if ( abs (temp_y -a[1] <100))
        if ( abs (temp_z - a[2] < 100))
        {
          Serial.println("error3"); //debug purpose
          return true;
        }
    Serial.println("temp_x diff" + (String) abs(temp_x - a[0]));
    Serial.println("temp_y diff" + (String) abs(temp_y - a[1]));
    Serial.println("temp_z diff" + (String) abs(temp_z - a[2]));    
    temp_x = a[0];
    temp_y = a[1];
    temp_z = a[2];
    
  }
  return false;
}

// if detect, then collab
void collab()
{
  delay (3000);
  while (true)
  {
  bool dec = detect();
  Serial.println("err1");
  if (dec == true){
    //song_1();
    song_2();
    //song_1();
    song_2();
    base.write (130);
    left_hit();
    left_hit();
    left_hit();

    
    
    break;
  }
  
  }
  song_2();
  song_1();
  song_2();
  song_1();
}
void action(bool manFlag, String msg){
  if(manFlag){
    manual();
  }
  else if (msg == "1"){
    song_1();
  }
  else if (msg == "2"){
    song_2();
  }
  else if(msg == "3"){
  song_3();
  }
  else if(msg == "4"){
  collab();
  }
}

void setup(){
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

  base.attach(D5);				//
  left.attach(D6);				//
  right.attach(D8);				//	set up servo
  left.write (0);				//
  
  Wire.begin(D2, D1);			//
  Wire.setClock(400000);		//	set up I2C connection with Gyro

  while (!initMPU()) {
   Serial.println("Init fail");
    delay(500);
  }
}

String temp="";

void loop() {

  WiFiClient client;
  while (client.connect(server, port) != 1)
  {
    Serial.println("connection failed! try again!");
    client.stop();
  }
  http.begin(ip, port, "/khoi/done");
  while (true) {
    do {
      getMsg();
      temp = msg.substring(0,4);
      Serial.println("loop1");
      gyro_read();
      Serial.println(a[0]);
    }
    while (temp != "manu" && temp != "auto" ) ;
    
    if (temp == "manu") manualFlag = true;
    else manualFlag = false;

    msg.remove(0, 4);
    Serial.println("loop2");
    
    
   

    // act according to flag
    // if manual act do single action depend on argument
    // if auto do predefine sequence of action depend on argument
    // function take in two arg(bool manualFlag, int action)
	action(manualFlag, msg);
	
	delay(250);
    Serial.println("loop");
  }

}

