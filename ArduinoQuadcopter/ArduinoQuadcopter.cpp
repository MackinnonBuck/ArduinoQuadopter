#include "ArduinoQuadcopter.h"

#define THROTTLE_DEADZONE		0.1

#define INPUT_PRIORITY			0.1

#define ESC_FRONT_LEFT_PIN		9
#define ESC_FRONT_RIGHT_PIN		10
#define ESC_REAR_LEFT_PIN		11
#define ESC_REAR_RIGHT_PIN		12

ESC frontLeftESC;
ESC frontRightESC;
ESC rearLeftESC;
ESC rearRightESC;

double xInput;
double xOutput;
double xSetpoint;

double yInput;
double yOutput;
double ySetpoint;

double kp = 1.0;
double ki = 0.0;
double kd = 0.0;

PID xPID(&xInput, &xOutput, &xSetpoint, kp, ki, kd, DIRECT);
PID yPID(&yInput, &yOutput, &ySetpoint, kp, ki, kd, DIRECT);

double trim(double value, double min, double max)
{
	return fmax(fmin(value, max), min);
}

double deadZone(double value, double dz)
{
	return value > dz ? value : 0.0;
}

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

	xPID.SetMode(AUTOMATIC);
	xPID.SetOutputLimits(-1.0, 1.0);

	yPID.SetMode(AUTOMATIC);
	yPID.SetOutputLimits(-1.0, 1.0);
}

void loop()
{
	if (!MPU6050Manager::getInstance()->isDmpReady()) return;

	// The MPU6050 has high priority because its interrupt triggers complicated algorithms
	while (!MPU6050Manager::getInstance()->hasInterrupted())
	{
		// Other program logic goes here

		ReceiverManager::getInstance()->update();

		xInput = (MPU6050Manager::getInstance()->getYawPitchRoll()[2] / 180.0) * (1.0 - INPUT_PRIORITY) +
				((ReceiverManager::getInstance()->getValues()[3] - 0.5) * 2.0) * INPUT_PRIORITY;

		yInput = (MPU6050Manager::getInstance()->getYawPitchRoll()[1] / 180.0) * (1.0 - INPUT_PRIORITY) +
				((ReceiverManager::getInstance()->getValues()[2] - 0.5) * 2.0) * INPUT_PRIORITY;

		xPID.Compute();
		yPID.Compute();

		if (ReceiverManager::getInstance()->getValues()[0] > THROTTLE_DEADZONE)
		{
			frontLeftESC.setValue(trim(-xOutput + yOutput + ReceiverManager::getInstance()->getValues()[0], 0.0, 1.0));
			frontRightESC.setValue(trim(xOutput + yOutput + ReceiverManager::getInstance()->getValues()[0], 0.0, 1.0));
			rearLeftESC.setValue(trim(-xOutput - yOutput + ReceiverManager::getInstance()->getValues()[0], 0.0, 1.0));
			rearRightESC.setValue(trim(xOutput - yOutput + ReceiverManager::getInstance()->getValues()[0], 0.0, 1.0));
		}
		else
		{
			frontLeftESC.setValue(0.0);
			frontRightESC.setValue(0.0);
			rearLeftESC.setValue(0.0);
			rearRightESC.setValue(0.0);
		}

		// Motor speed debugging
		/**/
		Serial.print("fl-fr-rl-rr\t");
		Serial.print(frontLeftESC.getValue());
		Serial.print("\t");
		Serial.print(frontRightESC.getValue());
		Serial.print("\t");
		Serial.print(rearLeftESC.getValue());
		Serial.print("\t");
		Serial.print(rearRightESC.getValue());
		Serial.println();
		/**/

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
