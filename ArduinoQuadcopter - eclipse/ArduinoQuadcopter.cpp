#include "ArduinoQuadcopter.h"

#define ESC_FRONT_LEFT_PIN					9
#define ESC_FRONT_RIGHT_PIN					10
#define ESC_REAR_LEFT_PIN					11
#define ESC_REAR_RIGHT_PIN					12

#define YAW_KP								0.1
#define YAW_KI								0.1
#define YAW_KD								0.0

#define PITCHROLL_KP						0.2
#define PITCHROLL_KI						0.1
#define PITCHROLL_KD						0.085

#define CONTROLLER_THROTTLE_DEADZONE		0.1
#define CONTROLLER_YAW_SENSITIVITY			1.0
#define CONTROLLER_PITCHROLL_SENSITIVITY	0.1

ESC frontLeftESC;
ESC frontRightESC;
ESC rearLeftESC;
ESC rearRightESC;

double yawInput;
double yawOutput;
double yawSetpoint;

double rollInput;
double rollOutput;
double rollSetpoint;

double pitchInput;
double pitchOutput;
double pitchSetpoint;

PID yawPID(&yawInput, &yawOutput, &yawSetpoint, YAW_KP, YAW_KI, YAW_KD, DIRECT);
PID rollPID(&rollInput, &rollOutput, &rollSetpoint, PITCHROLL_KP, PITCHROLL_KI, PITCHROLL_KD, REVERSE);
PID pitchPID(&pitchInput, &pitchOutput, &pitchSetpoint, PITCHROLL_KP, PITCHROLL_KI, PITCHROLL_KD, REVERSE);

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

	// Initailize ESCs
	frontLeftESC = ESC(ESC_FRONT_LEFT_PIN);
	frontRightESC = ESC(ESC_FRONT_RIGHT_PIN);
	rearLeftESC = ESC(ESC_REAR_LEFT_PIN);
	rearRightESC = ESC(ESC_REAR_RIGHT_PIN);

	// Initialize PID algorithms
	yawPID.SetMode(AUTOMATIC);
	yawPID.SetOutputLimits(-1.0, 1.0);
	yawPID.SetSampleTime(50);

	rollPID.SetMode(AUTOMATIC);
	rollPID.SetOutputLimits(-1.0, 1.0);
	rollPID.SetSampleTime(50);

	pitchPID.SetMode(AUTOMATIC);
	pitchPID.SetOutputLimits(-1.0, 1.0);
	pitchPID.SetSampleTime(50);
}

void loop()
{
	if (!MPU6050Manager::getInstance()->isDmpReady()) return;

	// The MPU6050 has high priority because its interrupt triggers complicated algorithms
	while (!MPU6050Manager::getInstance()->hasInterrupted())
	{
		ReceiverManager::getInstance()->update();

		if (ReceiverManager::getInstance()->getValues()[0] > CONTROLLER_THROTTLE_DEADZONE)
		{
			if (rollPID.GetMode() == MANUAL)
			{
				rollPID.SetMode(AUTOMATIC);
				pitchPID.SetMode(AUTOMATIC);
			}

			yawInput = MPU6050Manager::getInstance()->getRate()[0] / 360.0 +
					ReceiverManager::getInstance()->getValues()[1] * CONTROLLER_YAW_SENSITIVITY;

			rollInput = -(MPU6050Manager::getInstance()->getYawPitchRoll()[2] / 180.0) -
					((ReceiverManager::getInstance()->getValues()[3] - 0.5) * 2.0) * CONTROLLER_PITCHROLL_SENSITIVITY;

			pitchInput = -(MPU6050Manager::getInstance()->getYawPitchRoll()[1] / 180.0) -
					((ReceiverManager::getInstance()->getValues()[2] - 0.5) * 2.0) * CONTROLLER_PITCHROLL_SENSITIVITY;

			yawPID.Compute();
			rollPID.Compute();
			pitchPID.Compute();

			frontLeftESC.setValue(trim(-rollOutput + pitchOutput + yawOutput +
					ReceiverManager::getInstance()->getValues()[0], 0.0, 1.0));
			frontRightESC.setValue(trim(rollOutput + pitchOutput - yawOutput +
					ReceiverManager::getInstance()->getValues()[0], 0.0, 1.0));
			rearLeftESC.setValue(trim(-rollOutput - pitchOutput - yawOutput +
					ReceiverManager::getInstance()->getValues()[0], 0.0, 1.0));
			rearRightESC.setValue(trim(rollOutput - pitchOutput + yawOutput +
					ReceiverManager::getInstance()->getValues()[0], 0.0, 1.0));
		}
		else
		{
			if (rollPID.GetMode() == AUTOMATIC)
			{
				yawPID.SetMode(MANUAL);
				rollPID.SetMode(MANUAL);
				pitchPID.SetMode(MANUAL);
				yawOutput = 0.0;
				rollOutput = 0.0;
				pitchOutput = 0.0;
			}

			yawPID.Compute();
			rollPID.Compute();
			pitchPID.Compute();

			frontLeftESC.setValue(0.0);
			frontRightESC.setValue(0.0);
			rearLeftESC.setValue(0.0);
			rearRightESC.setValue(0.0);
		}

		/* ---------- DEBUGGING ---------- */

		// Motor speed debugging
		/** /
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

		// MPU6050 ypr debugging
		/** /
		Serial.print("ypr\t");
		for (int i = 0; i < 3; i++)
		{
			Serial.print(MPU6050Manager::getInstance()->getYawPitchRoll()[i]);
			Serial.print("\t");
		}
		Serial.println();
		/**/

		// MPU6050 rate debugging
		/** /
		Serial.print("rate (ypr)");
		for (int i = 0; i < 3; i++)
		{
			Serial.print(MPU6050Manager::getInstance()->getRate()[i]);
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
