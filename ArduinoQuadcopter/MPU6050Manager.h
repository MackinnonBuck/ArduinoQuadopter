/*
 * MPU6050Manager.h
 *
 *  Created on: Mar 8, 2015
 *      Author: Mackinnon Buck
 */

#ifndef MPU6050MANAGER_H_
#define MPU6050MANAGER_H_

#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

class MPU6050Manager {

private:

	static MPU6050Manager* m_pInstance;

	bool m_bInitialized;

	MPU6050 m_MPU6050;

	float m_ax;
	float m_ay;
	float m_az;
	float m_gx;
	float m_gy;
	float m_gz;

	MPU6050Manager();

	float scale(int16_t value, float min, float max);

public:

	static MPU6050Manager* getInstance();

	bool initialized();

	void initialize(int16_t xAccelOffset, int16_t yAccelOffset, int16_t zAccelOffset,
			int16_t xGyroOffset, int16_t yGyroOffset, int16_t zGyroOffset);
	void update();

	float getAccelX();
	float getAccelY();
	float getAccelZ();

	float getGyroX();
	float getGyroY();
	float getGyroZ();

};

#endif
