/*
 * MPU6050Manager.cpp
 *
 *  Created on: Apr 19, 2015
 *      Author: Mackinnon Buck
 */

#include "MPU6050_6Axis_MotionApps20.h"
#include "MPU6050Manager.h"

MPU6050Manager* MPU6050Manager::m_pInstance = 0;

MPU6050Manager::MPU6050Manager() : m_devStatus(0), m_mpuIntStatus(0), m_packetSize(0), m_fifoCount(0),
		m_dmpReady(false), m_mpuInterrupt(false)
{
}

void MPU6050Manager::dmpDataReady()
{
	MPU6050Manager::getInstance()->m_mpuInterrupt = true;
}

MPU6050Manager* MPU6050Manager::getInstance()
{
	if (m_pInstance == 0)
		m_pInstance = new MPU6050Manager();

	return m_pInstance;
}

uint8_t MPU6050Manager::initialize()
{
	#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
		Wire.begin();
		TWBR = 24;
	#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
		Fastwire::setup(400, true);
	#endif

	Serial.begin(115200);

	m_mpu.initialize();

	Serial.println(m_mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

	Serial.println("Initializing DMP...");
	m_devStatus = m_mpu.dmpInitialize();

	m_mpu.setXAccelOffset(-108);
	m_mpu.setYAccelOffset(1160);
	m_mpu.setZAccelOffset(2288);
	m_mpu.setXGyroOffset(28);
	m_mpu.setYGyroOffset(-28);
	m_mpu.setZGyroOffset(8);

	if (m_devStatus == 0)
	{
		Serial.println("Enabling DMP...");
		m_mpu.setDMPEnabled(true);

		Serial.println("Enabling interrupt detection...");
		attachInterrupt(0, dmpDataReady, RISING);
		m_mpuIntStatus = m_mpu.getIntStatus();

		Serial.println("Waiting for first interrupt...");
		m_dmpReady = true;

		m_packetSize = m_mpu.dmpGetFIFOPacketSize();
	}
	else
	{
		Serial.print("DMP initialization failed (code ");
		Serial.print(m_devStatus);
		Serial.println(")");
	}

	return m_devStatus;
}

bool MPU6050Manager::isDmpReady()
{
	return m_dmpReady;
}

bool MPU6050Manager::hasInterrupted()
{
	return m_mpuInterrupt || m_fifoCount >= m_packetSize;
}

void MPU6050Manager::update()
{
	m_mpuInterrupt = false;
	m_mpuIntStatus = m_mpu.getIntStatus();

	m_fifoCount = m_mpu.getFIFOCount();

	if ((m_mpuIntStatus & 0x10) || m_fifoCount == 1024)
	{
		m_mpu.resetFIFO();
		Serial.println("FIFO overflow!");
	}
	else if (m_mpuIntStatus & 0x02)
	{
		while (m_fifoCount < m_packetSize) m_fifoCount = m_mpu.getFIFOCount();

		m_mpu.getFIFOBytes(m_fifoBuffer, m_packetSize);

		m_fifoCount -= m_packetSize;

		Quaternion q;
		m_mpu.dmpGetQuaternion(&q, m_fifoBuffer);
		Serial.print("quat\t");
		Serial.print(q.w);
		Serial.print("\t");
		Serial.print(q.x);
		Serial.print("\t");
		Serial.print(q.y);
		Serial.print("\t");
		Serial.println(q.z);

		// TODO: Turn these quaternion values into ypr angles.
	}
}
