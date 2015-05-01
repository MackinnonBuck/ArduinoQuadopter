#include "ArduinoQuadcopter.h"

void setup()
{
	// Initialize the MPU6050Manager
	MPU6050Manager::getInstance()->initialize(-108, 1160, 2288, 28, -28, 8);
}

void loop()
{
	if (!MPU6050Manager::getInstance()->isDmpReady()) return;

	while (!MPU6050Manager::getInstance()->hasInterrupted())
	{
		// Other program logic goes here
		Serial.print("ypr\t");
		Serial.print(MPU6050Manager::getInstance()->getYawPitchRoll()[0]);
		Serial.print("\t");
		Serial.print(MPU6050Manager::getInstance()->getYawPitchRoll()[1]);
		Serial.print("\t");
		Serial.println(MPU6050Manager::getInstance()->getYawPitchRoll()[2]);
	}

	// Update the MPU6050Manager's values
	MPU6050Manager::getInstance()->update();
}
