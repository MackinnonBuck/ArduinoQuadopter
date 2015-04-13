#include "ArduinoQuadcopter.h"

#define DEBUG

void setup()
{
	Serial.begin(38400);

	MPU6050Manager::getInstance()->initialize(-108, 1160, 2288, 28, -28, 8);

	#ifdef DEBUG
		Serial.println(MPU6050Manager::getInstance()->initialized() ?
				"MPU6050 initialization successful" : "MPU6050 initialization failed");
	#endif
}

void loop()
{
	MPU6050Manager::getInstance()->update();

	Serial.print("ax:\t");
	Serial.print(abs(MPU6050Manager::getInstance()->getAccelX()));
	Serial.print("\tay:\t");
	Serial.print(abs(MPU6050Manager::getInstance()->getAccelY()));
	Serial.print("\taz:\t");
	Serial.print(abs(MPU6050Manager::getInstance()->getAccelZ()));
	Serial.print("\tgx:\t");
	Serial.print(abs(MPU6050Manager::getInstance()->getGyroX()));
	Serial.print("\tgy:\t");
	Serial.print(abs(MPU6050Manager::getInstance()->getGyroY()));
	Serial.print("\tgz:\t");
	Serial.print(abs(MPU6050Manager::getInstance()->getGyroZ()));
	Serial.println();

	delay(100);
}
