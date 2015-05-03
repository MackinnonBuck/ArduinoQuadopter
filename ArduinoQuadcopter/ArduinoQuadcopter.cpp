#include "ArduinoQuadcopter.h"

void setup()
{
	Serial.begin(115200);

	// Initialize the MPU6050Manager
	MPU6050Manager::getInstance()->initialize(-108, 1160, 2288, 28, -28, 8);
	ReceiverManager::getInstance()->initialize();
}

void loop()
{
	if (!MPU6050Manager::getInstance()->isDmpReady()) return;

	// The MPU6050 has high priority because its interrupt triggers complicated algorithms
	while (!MPU6050Manager::getInstance()->hasInterrupted())
	{
		// Other program logic goes here

		ReceiverManager::getInstance()->update();

		// MPU6050 debugging

		/** /
		Serial.print("ypr\t");
		for (int i = 0; i < 3; i++)
		{
			Serial.print(MPU6050Manager::getInstance()->getYawPitchRoll()[i]);
			Serial.print("\t");
		}
		Serial.println();
		/**/

		// Receiver debugging

		/** /
		Serial.print("typr:\t");
		for (int i = 0; i < PIN_COUNT; i++)
		{
			Serial.print(ReceiverManager::getInstance()->getValues()[i]);
			Serial.print("\t");
		}
		Serial.println();
		/**/
	}

	MPU6050Manager::getInstance()->update();
}
