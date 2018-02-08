# EE183DA_Lab2
Project Making Noise
This tutorial will show you how to build musical network by using 3 ESP8266 MCU. One acts as control server and the reset is capable of various joint movement to fit your instrument need. 

<img width="600" alt="Setup" src="https://github.com/mqc25/EE183/blob/master/img/IMG_20180207_235736.jpg">

## Team Member:

- Cuong Hoang - 004847900
- Khoi Luc - 104570581
- Minh Cao	- 804844568

## The Demo:

Press CC to see our description of demo

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/8fS7rfw3ZuM/0.jpg)](https://www.youtube.com/watch?v=8fS7rfw3ZuM)

## Bill of Materials:
- 3 x ESP8266 MCU
- Breadboard
- Lots of Wires
- Micro-USB Cables
- 5 x Servo Motor
- 3 x Touch pen
- Touch sever
- MPU-6000 Gyro-Accelerometer sensor
- Instrument of choice (preferable touch interactive device)

## Basic functionality:

This a musical band WiFi-network that has one MCU act as control server that provide user interface option on webpage and send command to clients accordingly 
depend on user inputs. 

Available options are:
- **Manual**: Control the swing of servo via touch sensor or gyro sensor.
- **Play_song_(1/2/3)** (for 1st or 2nd node): Each node has a preset song rhythm built in that can be play individually
- **Collaborate**: The two node play together, 1 will play first then signal other to play via gyro and touch sensor then both play together.
		
This project currently has 1 server and 2 clients. One with 2 servos and one with 3 servos. One serves as the base to adjust the angle
while the others to swing touch pen to play a node. 
They either have touch sensor or gyro-accelerometer sensor to allow manual control and collaboration.
	
## Hardware Setup:

- Connect the circuit as shown below:

<img width="600" alt="Setup" src="https://github.com/mqc25/EE183/blob/master/img/Servo1-Schematic.png">

<img width="600" alt="Setup" src="https://github.com/mqc25/EE183/blob/master/img/Servo2-Schematic.png">

- Use 1 NodeMCU leftover to set up server to control the rest.
- Mount your choice of stick to the top motor.
- Attach servo to stable ground for better swing quality.
- Adjust your rhythm of choice as software description below.
- Enjoy! Be creative!

#### Note on servo calibration:

It is important to calibrate the servo so one can swing to signal others as well as making a stable ground for servo to stand on. 
The angle of swing will be determined by your instrument of choice and the sound you want to make. Play with it. Be creative. 
	
## Software setup:

Description of code functionality available in code file. 

#### Necessary library for this project:
- ESP8266WiFi.h
- ESP8266HTTPClient.h
- WiFiClient.h
- ESP8266WebServer.h
- Servo.h
- MPU6050.h
- Wire.h
- I2Cdev.h

#### Note on coding option:

**Server:** 
- Network id, password, client and user port number can be changed at the beginning of the code:

```
const char* ssid = "<new_ssid>";					// AP ssid
const char* password = "<password>";				// AP password

const int port = <port_number>;						// web browser port
const int port1 = <port_number>;					// client 1 port
const int port2 = <port_number>;					// client 2 port
```

- Additional options at server can be added as follow:

-- Adding option to webpage:
```
s += "<br><input type=\"button\" name=\"b1\" value=\"Manual\"";
s += " onclick=\"location.href='/MANUAL'\">";
s += "<br><br><br>";
```

-- Adding option to server:
```
 else if (request.indexOf("/NEW_OPTION") != -1)
  {
    Serial.println("NEW_OPTION");
    clientcommand[1] = "<auto/manual><option_num>";

  }
```

**Client:** 
- Add new song:
```
int newsong1[]={<int>,...}; 				//	#of time to hit the drum at current pos
int newsong2[]={<angle>,...};				//	angle of swing 1=120, 2 = 90, 3 = 45
											// 	the two array must have same size

void playnewsong() // play newsong function		
{
  start();
  for(int i = 0; i < sizeof(newsong1)/sizeof(newsong1[0]); i++)
    {
      play(newsong1[i], newsong2[i]);
       delay(30);
    }
    Serial.println("DONE newsong");
    stopservo();
 }
 
void decide(bool manFlag, String msg){		// assign this song to an id from server
  ...
  else if(msg=="<song_num>"){
    playnewsong();
  }
  ...
}
```
