#include "ArduinoQuadcopter.h"

#define ESC_FRONT_LEFT_PIN		9
#define ESC_FRONT_RIGHT_PIN		10
#define ESC_REAR_LEFT_PIN		11
#define ESC_REAR_RIGHT_PIN		12

ESC frontLeftESC;
ESC frontRightESC;
ESC rearLeftESC;
ESC rearRightESC;

void setup()
{
	Serial.begin(115200);

	// Initialize the MPU6050Manager
	MPU6050Manager::getInstance()->initialize(-108, 1160, 2288, 28, -28, 8);

	// Initialize the ReceiverManager
	ReceiverManager::getInstance()->initialize();

	frontLeftESC = ESC(ESC_FRONT_LEFT_PIN);
	frontRightESC = ESC(ESC_FRONT_RIGHT_PIN);
	rearLeftESC = ESC(ESC_REAR_LEFT_PIN);
	rearRightESC = ESC(ESC_REAR_RIGHT_PIN);
}

void loop()
{
	if (!MPU6050Manager::getInstance()->isDmpReady()) return;

	// The MPU6050 has high priority because its interrupt triggers complicated algorithms
	while (!MPU6050Manager::getInstance()->hasInterrupted())
	{
		// Other program logic goes here

		ReceiverManager::getInstance()->update();

		frontLeftESC.setValue(ReceiverManager::getInstance()->getValues()[0]);
		frontRightESC.setValue(ReceiverManager::getInstance()->getValues()[0]);
		rearLeftESC.setValue(ReceiverManager::getInstance()->getValues()[0]);
		rearRightESC.setValue(ReceiverManager::getInstance()->getValues()[0]);

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
