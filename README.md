# EE183DA_Lab2
Project Making Noise
This tutorial will show you how to build musical network by using 3 ESP8266 MCU. One acts as control server and the reset is capable of various joint movement to fit your instrument need. 

<img width="600" alt="Setup" src="https://ibb.co/cNmiac">

## The Demo:

Press CC to see our description of demo

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/8fS7rfw3ZuM/0.jpg)](https://www.youtube.com/watch?v=8fS7rfw3ZuM)

## Bill of Materials:
- 3 x ESP8266 MCU
- Micro-USB Cable
- 5 x Servo Motor
- 3 x Touch pen
- Instrument of choice (preferable touch interactive device)

## Basic functionality:

The server will set up the access point for the rest of the system to connect to and receive command from. User can connect to this server via browser and choose the option available on the screen include:

- **Manual**: Control the swing of servo via touch sensor or gyro sensor.
- **Play_song_(1/2/3)** (for 1st or 2nd node): Each node has a preset song rhythm built in that can be play individually
- **Collaborate**: The two node play together, 1 will play first then signal other to play via gyro and touch sensor then both play together.
		
The Node: has 2 or 3 servos. One serve as base to adjust the angle, the other to swing to play a node. They either have touch sensor or gyro to allow manual control and collaboration.
	
## Hardware Setup:

- Connect the circuit as shown below:

<img width="600" alt="Setup" src="https://ibb.co/jVc0MH">

<img width="600" alt="Setup" src="https://ibb.co/g0fn1H">

- Use 1 NodeMCU leftover to set up server to control the rest.
- Tie your choice of stick to the top motor.
- Adjust your rhythm of choice
- Enjoy. Be creative

## Software setup:

#### Necessary library:
- ESP8266WiFi.h
- ESP8266HTTPClient.h
- WiFiClient.h
- ESP8266WebServer.h
- Servo.h
- MPU6050.h
- Wire.h
- I2Cdev.h

#### Calibrate the servo:

It is important to calibrate the servo so one can swing to signal others as well as making a stable ground for servo to stand on. The angle of swing will be determined by your instrument of choice and the sound you want to make. Play with it. Be creative. 
	


