/*
 * ArduinoQuadcopter.ino
 *
 *  Created on: May 15, 2015
 *      Author: Mackinnon Buck
 */

/*
 * TODO: Change the receiver programming to accommodate for the new control loop
 */

#include <Servo.h>
#include <Wire.h>
#include "PID_v1.h"
#include "MPU6050Manager.h"
#include "ReceiverManager.h"
#include "ESC.h"

#define OFFSET_ACCEL_X                                          -108
#define OFFSET_ACCEL_Y                                          1160
#define OFFSET_ACCEL_Z                                          2288
#define OFFSET_GYRO_X                                           28
#define OFFSET_GYRO_Y                                           -28
#define OFFSET_GYRO_Z                                           8

#define ESC_FRONT_LEFT_PIN					9
#define ESC_FRONT_RIGHT_PIN					10
#define ESC_REAR_LEFT_PIN					11
#define ESC_REAR_RIGHT_PIN					12

#define YAW_KP							0.1
#define YAW_KI							0.1
#define YAW_KD							0.0

#define PITCHROLL_KP						0.2
#define PITCHROLL_KI						0.1
#define PITCHROLL_KD						0.075

#define CONTROLLER_THROTTLE_DEADZONE		                0.1
#define CONTROLLER_YAW_SENSITIVITY			        1.5
#define CONTROLLER_PITCHROLL_SENSITIVITY	                0.1

#define LOOP_TARGET_DURATION                                    20

// Uncomment a debug macro definition to enable its corresponding debug process
#define DEBUG_MOTORSPEED
//#define DEBUG_IMU_YPR
//#define DEBUG_IMU_RATE
//#define DEBUG_IMU_GRAVITY
//#define DEBUG_RECEIVER

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

unsigned long loopStartTime;
unsigned long loopDuration;

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
  MPU6050Manager::getInstance()->initialize(OFFSET_ACCEL_X, OFFSET_ACCEL_Y, OFFSET_ACCEL_Z, OFFSET_GYRO_X, OFFSET_GYRO_Y, OFFSET_GYRO_Z);

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
  yawPID.SetSampleTime(LOOP_TARGET_DURATION);

  rollPID.SetMode(AUTOMATIC);
  rollPID.SetOutputLimits(-1.0, 1.0);
  rollPID.SetSampleTime(LOOP_TARGET_DURATION);

  pitchPID.SetMode(AUTOMATIC);
  pitchPID.SetOutputLimits(-1.0, 1.0);
  pitchPID.SetSampleTime(LOOP_TARGET_DURATION);
}

void loop()
{
  loopStartTime = millis();
  
  if (!MPU6050Manager::getInstance()->initSuccessful()) return;

  MPU6050Manager::getInstance()->update();
  ReceiverManager::getInstance()->update();

  if (ReceiverManager::getInstance()->getValues()[0] > CONTROLLER_THROTTLE_DEADZONE)
  {
    if (rollPID.GetMode() == MANUAL)
    {
      yawPID.SetMode(AUTOMATIC);
      rollPID.SetMode(AUTOMATIC);
      pitchPID.SetMode(AUTOMATIC);
    }

    yawInput = (MPU6050Manager::getInstance()->getRate()[0] / 360.0 +
                (ReceiverManager::getInstance()->getValues()[1] - 0.5) * 2.0) * CONTROLLER_YAW_SENSITIVITY;

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
#ifdef DEBUG_MOTORSPEED
  Serial.print("fl-fr-rl-rr\t");
  Serial.print(frontLeftESC.getValue());
  Serial.print("\t");
  Serial.print(frontRightESC.getValue());
  Serial.print("\t");
  Serial.print(rearLeftESC.getValue());
  Serial.print("\t");
  Serial.print(rearRightESC.getValue());
  Serial.println();
#endif

  // MPU6050 ypr debugging
#ifdef DEBUG_IMU_YPR
  Serial.print("ypr\t");
  for (int i = 0; i < 3; i++)
  {
    Serial.print(MPU6050Manager::getInstance()->getYawPitchRoll()[i]);
    Serial.print("\t");
  }
  Serial.println();
#endif

  // MPU6050 rate debugging
#ifdef DEBUG_IMU_RATE
  Serial.print("rate\t");
  for (int i = 0; i < 3; i++)
  {
    Serial.print(MPU6050Manager::getInstance()->getRate()[i]);
    Serial.print("\t");
  }
  Serial.println();
#endif

#ifdef DEBUG_IMU_GRAVITY
  Serial.print("gravity\t");
  for (int i = 0; i < 3; i++)
  {
    Serial.print(MPU6050Manager::getInstance()->getGravity()[i]);
    Serial.print("\t");
  }
  Serial.println();
#endif

  // Receiver debugging
#ifdef DEBUG_RECEIVER
  Serial.print("typr:\t");
  for (int i = 0; i < PIN_COUNT; i++)
  {
    Serial.print(ReceiverManager::getInstance()->getValues()[i]);
    Serial.print("\t");
  }
  Serial.println();
#endif

  loopDuration = millis() - loopStartTime;
  if (loopDuration < LOOP_TARGET_DURATION)
  {
    delay(LOOP_TARGET_DURATION - loopDuration);
  }
}
