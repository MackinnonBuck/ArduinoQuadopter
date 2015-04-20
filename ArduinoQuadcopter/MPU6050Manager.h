/*
 * MPU6050Manager.h
 *
 *  Created on: Apr 19, 2015
 *      Author: Mackinnon Buck
 */

#ifndef ARDUINOQUADCOPTER_OLD_MPU6050MANAGER_H_
#define ARDUINOQUADCOPTER_OLD_MPU6050MANAGER_H_

#include "MPU6050.h"

class MPU6050Manager {

private:

	static MPU6050Manager* m_pInstance;

	MPU6050 m_mpu;

	uint8_t m_devStatus;
	uint8_t m_mpuIntStatus;
	uint8_t m_fifoBuffer[64];

	uint16_t m_packetSize;
	uint16_t m_fifoCount;

	bool m_dmpReady;
	volatile bool m_mpuInterrupt;

	MPU6050Manager();

	static void dmpDataReady();

public:

	static MPU6050Manager* getInstance();

	uint8_t initialize();

	bool isDmpReady();
	bool hasInterrupted();

	void update();
};

#endif /* ARDUINOQUADCOPTER_OLD_MPU6050MANAGER_H_ */
