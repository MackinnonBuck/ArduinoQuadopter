/*
 * MPU6050Manager.h
 *
 *  Created on: Apr 19, 2015
 *      Author: Mackinnon Buck
 */

#ifndef _MPU6050MANAGER_H_
#define _MPU6050MANAGER_H_

#include "MPU6050.h"

class MPU6050Manager {

private:

	static MPU6050Manager* m_pInstance;

	MPU6050 m_mpu;

	uint8_t m_devStatus;
	uint8_t m_mpuIntStatus;
	uint8_t m_fifoBuffer[64];
	uint8_t* m_pPacket[8];

	uint16_t m_packetSize;
	uint16_t m_fifoCount;

	float m_quat[4];
	float m_gravity[3];
	float m_ypr[3];

	bool m_dmpReady;
	volatile bool m_mpuInterrupt;

	MPU6050Manager();

	static void dmpDataReady();

	void extendAngle(float& angle);

public:

	static MPU6050Manager* getInstance();

	uint8_t initialize(int16_t xAccelOffset, int16_t yAccelOffset, int16_t zAccelOffset,
			int16_t xGyroOffset, int16_t yGyroOffset, int16_t zGyroOffset);

	bool isDmpReady();
	bool hasInterrupted();

	void update();

	float* getQuaternion();
	float* getGravity();
	float* getYawPitchRoll();
};

#endif /* ARDUINOQUADCOPTER_OLD_MPU6050MANAGER_H_ */
