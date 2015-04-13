/*
 * MPU6050Manager.cpp
 *
 *  Created on: Mar 8, 2015
 *      Author: Mackinnon Buck
 */

#include "MPU6050Manager.h"
#include "Arduino.h"

MPU6050Manager* MPU6050Manager::m_pInstance = 0;

MPU6050Manager::MPU6050Manager() : m_bInitialized(false), m_MPU6050(0x68),
		m_ax(0), m_ay(0), m_az(0), m_gx(0), m_gy(0), m_gz(0)
{
}

float MPU6050Manager::scale(int16_t value, float min, float max)
{
	return (value + 32768.0f) * (max - min) / (32767.0f +32768.0f) + min;
}

MPU6050Manager* MPU6050Manager::getInstance()
{
	if (m_pInstance == 0)
		m_pInstance = new MPU6050Manager();

	return m_pInstance;
}

bool MPU6050Manager::initialized()
{
	return m_bInitialized;
}

void MPU6050Manager::initialize(int16_t xAccelOffset, int16_t yAccelOffset, int16_t zAccelOffset,
		int16_t xGyroOffset, int16_t yGyroOffset, int16_t zGyroOffset)
{
	#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
		Wire.begin();
	#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
		Fastwire::setup(400, true);
	#endif

	m_MPU6050.initialize();
	m_bInitialized = m_MPU6050.testConnection();

	if (m_bInitialized)
	{
		m_MPU6050.setXAccelOffset(xAccelOffset);
		m_MPU6050.setYAccelOffset(yAccelOffset);
		m_MPU6050.setZAccelOffset(zAccelOffset);
		m_MPU6050.setXGyroOffset(xGyroOffset);
		m_MPU6050.setYGyroOffset(yGyroOffset);
		m_MPU6050.setZGyroOffset(zGyroOffset);
	}
}

void MPU6050Manager::update()
{
	m_ax = scale(m_MPU6050.getAccelerationX(), -1.0, 1.0);
	m_ay = scale(m_MPU6050.getAccelerationY(), -1.0, 1.0);
	m_az = scale(m_MPU6050.getAccelerationZ(), -1.0, 1.0);
	m_gx = scale(m_MPU6050.getRotationX(), -1.0, 1.0);
	m_gy = scale(m_MPU6050.getRotationY(), -1.0, 1.0);
	m_gz = scale(m_MPU6050.getRotationZ(), -1.0, 1.0);
}

float MPU6050Manager::getAccelX()
{
	return m_ax;
}

float MPU6050Manager::getAccelY()
{
	return m_ay;
}

float MPU6050Manager::getAccelZ()
{
	return m_az;
}

float MPU6050Manager::getGyroX()
{
	return m_gx;
}

float MPU6050Manager::getGyroY()
{
	return m_gy;
}

float MPU6050Manager::getGyroZ()
{
	return m_gz;
}
