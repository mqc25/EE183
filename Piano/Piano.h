#ifndef MPU_CONFIG_H
#define MPU_CONFIG_H

#include "MPU6050_6Axis_MotionApps20.h"
#include "MPU6050.h"



#define LPF 	  4
#define HPF 		1
#define GYRO_RNG 	2
#define ACC_RNG 	3

#define MOT_THRESHOLD 	5
#define MOT_DUR			10
#define MOT_DECREMENT	1
#define INT_MODE		1
#define INT_LATCH		1
#define INT_EN			1
#define LATCH_CLR		0

#define	FF_EN			  0
#define MOT_EN			1
#define ZERO_EN			0
#define FIFO_EN			0
#define	MASTER_EN		0
#define RDY_EN			0


extern volatile bool state;
extern int16_t ax,ay,az,gx,gy,gz;
extern MPU6050 mpu;

bool initMPU();
void detectMotion();
bool checkMotion();
String MPU_Status();
String getMPUReading_gyro();
double* getMPUReading_acc();
#endif
