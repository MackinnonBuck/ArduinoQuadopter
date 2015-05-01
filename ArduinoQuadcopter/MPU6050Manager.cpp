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
	m_pPacket[0] = &m_fifoBuffer[0];
    m_pPacket[1] = &m_fifoBuffer[1];
    m_pPacket[2] = &m_fifoBuffer[4];
    m_pPacket[3] = &m_fifoBuffer[5];
    m_pPacket[4] = &m_fifoBuffer[8];
    m_pPacket[5] = &m_fifoBuffer[9];
    m_pPacket[6] = &m_fifoBuffer[12];
    m_pPacket[7] = &m_fifoBuffer[13];
}

void MPU6050Manager::dmpDataReady()
{
	MPU6050Manager::getInstance()->m_mpuInterrupt = true;
}

/*
 * Increases the pitch and roll range from -90 and 90 to -180 and 180
 */
void MPU6050Manager::extendAngle(float& angle)
{
	if (m_gravity[2] < 0)
	{
		if (angle > 0)
			angle = 180 - angle;
		else if (m_ypr[1] < 0)
			angle = -180 - angle;
	}
}

/*
 * Returns the global MPU6050Manager instance
 */
MPU6050Manager* MPU6050Manager::getInstance()
{
	if (m_pInstance == 0)
		m_pInstance = new MPU6050Manager();

	return m_pInstance;
}

/*
 * Initializes the MPU6050 with specific offsets
 */
uint8_t MPU6050Manager::initialize(int16_t xAccelOffset, int16_t yAccelOffset, int16_t zAccelOffset,
		int16_t xGyroOffset, int16_t yGyroOffset, int16_t zGyroOffset)
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

	m_mpu.setXAccelOffset(xAccelOffset);
	m_mpu.setYAccelOffset(yAccelOffset);
	m_mpu.setZAccelOffset(zAccelOffset);
	m_mpu.setXGyroOffset(xGyroOffset);
	m_mpu.setYGyroOffset(yGyroOffset);
	m_mpu.setZGyroOffset(zGyroOffset);

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

		// Get quaternion values
        m_quat[0] = ((*m_pPacket[0] << 8) | *m_pPacket[1]) / 16384.0f;
        m_quat[1] = ((*m_pPacket[2] << 8) | *m_pPacket[3]) / 16384.0f;
        m_quat[2] = ((*m_pPacket[4] << 8) | *m_pPacket[5]) / 16384.0f;
        m_quat[3] = ((*m_pPacket[6] << 8) | *m_pPacket[7]) / 16384.0f;
        for (int i = 0; i < 4; i++) if (m_quat[i] >= 2.0f) m_quat[i] = -4.0f + m_quat[i];

        // Get gravity calculations
        m_gravity[0] = 2 * (m_quat[1] * m_quat[3] - m_quat[0] * m_quat[2]);
        m_gravity[1] = 2 * (m_quat[0] * m_quat[1] + m_quat[2] * m_quat[3]);
        m_gravity[2] = m_quat[0] * m_quat[0] - m_quat[1] * m_quat[1] -
        		m_quat[2] * m_quat[2] + m_quat[3] * m_quat[3];

        // Gey ypr values
	    m_ypr[0] = atan2(2*m_quat[1]*m_quat[2] - 2*m_quat[0]*m_quat[3],
	    		2*m_quat[0]*m_quat[0] + 2*m_quat[1]*m_quat[1] - 1)*180.0f/PI;
	    m_ypr[1] = atan(m_gravity[0] / sqrt(m_gravity[1]*m_gravity[1] + m_gravity[2]*m_gravity[2]))*180.0f/PI;
	    m_ypr[2] = atan(m_gravity[1] / sqrt(m_gravity[0]*m_gravity[0] + m_gravity[2]*m_gravity[2]))*180.0f/PI;

	    // Increase the pitch and roll range from -90 and 90 to -180 and 180
	    extendAngle(m_ypr[1]);
	    extendAngle(m_ypr[2]);
	}
}

/*
 * Return the calculated quaternion values
 */
float* MPU6050Manager::getQuaternion()
{
	return m_quat;
}

/*
 * Return the calculated gravity values
 */
float* MPU6050Manager::getGravity()
{
	return m_gravity;
}

/*
 * Return the calculated yaw, pitch, and roll values
 */
float* MPU6050Manager::getYawPitchRoll()
{
	return m_ypr;
}
