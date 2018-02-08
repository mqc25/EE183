#include "mpu_config.h"


volatile bool state = false;
MPU6050 mpu(MPU6050_ADDRESS_AD0_HIGH); 
int16_t ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz =0;

bool initMPU(){
	mpu.initialize();
 
	if(!mpu.testConnection()){
		return false;
	}
  
	mpu.setDLPFMode(LPF);                       // 20 Hz
	mpu.setDHPFMode(HPF);                       // 5 Hz
	mpu.setFullScaleGyroRange(GYRO_RNG);             // 1000 deg/s
	mpu.setFullScaleAccelRange(ACC_RNG);            // 8 g

	mpu.setMotionDetectionThreshold(MOT_THRESHOLD);      // 100 mg
	mpu.setMotionDetectionDuration(MOT_DUR);      // 100 ms
	mpu.setMotionDetectionCounterDecrement(MOT_DECREMENT); // 0 reset
	mpu.setInterruptMode(INT_MODE);                  // active low
	mpu.setInterruptLatch(INT_LATCH);                 // 50 us pulse
	mpu.setIntEnabled(INT_EN);                     // enable INT
	mpu.setInterruptLatchClear(LATCH_CLR);
	
	mpu.setIntFreefallEnabled(FF_EN);
	mpu.setIntMotionEnabled(MOT_EN);
	mpu.setIntZeroMotionEnabled(ZERO_EN);
	mpu.setIntFIFOBufferOverflowEnabled(FIFO_EN);
	mpu.setIntI2CMasterEnabled(MASTER_EN);
	mpu.setIntDataReadyEnabled(RDY_EN);

  return true;
}

void detectMotion(){
	state = true;
}

bool checkMotion(){
	if(state == true){
		state = false;
	}
	return mpu.getIntMotionStatus();
}

double* getMPUReading_acc(){
	//String a= "";
	mpu.getMotion6(&ax,&ay,&az,&gx,&gy,&gz);
  /*
	a = "ax: " + String(ax) + " " + "ay: " + String(ay) + " " + "az: " + String(az) + " " 
		"gx: " + String(gx) + " " + "gy: " + String(gy) + " " + "gz: " + String(gz);

    */
    //a = "ax: " + String(ax) + " " + "ay: " + String(ay) + " " + "az: " + String(az);
	double *a = new double[4];
  a[0] = ax;
  a[1]=ay;
  a[2] =az;
	return a;
}

String getMPUReading_gyro(){
  String a= "";
  mpu.getMotion6(&ax,&ay,&az,&gx,&gy,&gz);
  /*
  a = "ax: " + String(ax) + " " + "ay: " + String(ay) + " " + "az: " + String(az) + " " 
    "gx: " + String(gx) + " " + "gy: " + String(gy) + " " + "gz: " + String(gz);

    */
    a = "gx: " + String(gx) + " " + "gy: " + String(gy) + " " + "gz: " + String(gz);
  return a;
}
String MPU_Status(){
	
	
	String out = "";
	out += "LPF :" + String(mpu.getDLPFMode()) + "\n";
	out += "HPF :" + String(mpu.getDHPFMode()) + "\n";
	out += "Gyro Range :" + String(mpu.getFullScaleGyroRange()) + "\n";
	out += "Acc Range :" + String(mpu.getFullScaleAccelRange()) + "\n";
	out += "Motion Threshold :" + String(mpu.getMotionDetectionThreshold()) + "\n";
	out += "Motion Dur Threshold : :" + String(mpu.getMotionDetectionDuration()) + "\n";
	out += "Interrupt Mode :" + String(mpu.getInterruptMode()) + "\n";
	out += "Latch Mode :" + String(mpu.getInterruptLatch()) + "\n";
	out += "Int enable :" + String(mpu.getIntEnabled()) + "\n";
	out += "Counter Motion Decrement :" + String(mpu.getMotionDetectionCounterDecrement()) + "\n";
	out += "----------------------------------------------------------\n";
 
	return out;
}
