/*
 * MPU6050Manager.cpp
 *
 *  Created on: Apr 19, 2015
 *      Author: Mackinnon Buck
 */

#include "MPU6050Manager.h"

MPU6050Manager* MPU6050Manager::m_pInstance = 0;

MPU6050Manager::MPU6050Manager() :
		m_devStatus(0), m_mpuIntStatus(0), m_packetSize(0), m_fifoCount(0), m_dmpReady(
		false), m_mpuInterrupt(false)
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

uint8_t MPU6050Manager::dmpInitialize()
{
	// reset device
	m_mpu.reset();
	delay(30); // wait after reset

	m_mpu.setSleepEnabled(false);

	// get MPU hardware revision
	m_mpu.setMemoryBank(0x10, true, true);
	m_mpu.setMemoryStartAddress(0x06);
	uint8_t hwRevision = m_mpu.readMemoryByte();
	m_mpu.setMemoryBank(0, false, false);

	// check OTP bank valid
	uint8_t otpValid = m_mpu.getOTPBankValid();

	// get X/Y/Z gyro offsets
	int8_t xgOffsetTC = m_mpu.getXGyroOffsetTC();
	int8_t ygOffsetTC = m_mpu.getYGyroOffsetTC();
	int8_t zgOffsetTC = m_mpu.getZGyroOffsetTC();

	// setup slave addresses
	m_mpu.setSlaveAddress(0, 0x7F);
	m_mpu.setI2CMasterModeEnabled(false);
	m_mpu.setSlaveAddress(0, 0x68);
	m_mpu.resetI2CMaster();
	delay(20);

	// load DMP code into memory banks
	if (m_mpu.writeProgMemoryBlock(dmpMemory, MPU6050_DMP_CODE_SIZE))
	{
		if (m_mpu.writeProgDMPConfigurationSet(dmpConfig,
		MPU6050_DMP_CONFIG_SIZE))
		{
			m_mpu.setClockSource(MPU6050_CLOCK_PLL_ZGYRO);

			m_mpu.setIntEnabled(0x12);

			m_mpu.setRate(4); // 1khz / (1 + 4) = 200 Hz

			m_mpu.setExternalFrameSync(MPU6050_EXT_SYNC_TEMP_OUT_L);

			m_mpu.setDLPFMode(MPU6050_DLPF_BW_42);

			m_mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);

			m_mpu.setDMPConfig1(0x03);
			m_mpu.setDMPConfig2(0x00);

			m_mpu.setOTPBankValid(false);

			m_mpu.setXGyroOffsetTC(xgOffsetTC);
			m_mpu.setYGyroOffsetTC(ygOffsetTC);
			m_mpu.setZGyroOffsetTC(zgOffsetTC);

			uint8_t dmpUpdate[16], j;
			uint16_t pos = 0;
			for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
				dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
			m_mpu.writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0],
					dmpUpdate[1]);

			for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
				dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
			m_mpu.writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0],
					dmpUpdate[1]);

			m_mpu.resetFIFO();

			uint16_t fifoCount = m_mpu.getFIFOCount();
			uint8_t fifoBuffer[128];

			m_mpu.getFIFOBytes(fifoBuffer, fifoCount);

			m_mpu.setMotionDetectionThreshold(2);

			m_mpu.setZeroMotionDetectionThreshold(156);

			m_mpu.setMotionDetectionDuration(80);

			m_mpu.setZeroMotionDetectionDuration(0);

			m_mpu.resetFIFO();

			m_mpu.setFIFOEnabled(true);

			m_mpu.setDMPEnabled(true);

			m_mpu.resetDMP();

			for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
				dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
			m_mpu.writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0],
					dmpUpdate[1]);

			for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
				dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
			m_mpu.writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0],
					dmpUpdate[1]);

			for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
				dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
			m_mpu.writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0],
					dmpUpdate[1]);

			while ((fifoCount = m_mpu.getFIFOCount()) < 3);

			m_mpu.getFIFOBytes(fifoBuffer, fifoCount);

			uint8_t mpuIntStatus = m_mpu.getIntStatus();

			for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
				dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
			m_mpu.readMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0],
					dmpUpdate[1]);

			while ((fifoCount = m_mpu.getFIFOCount()) < 3);

			m_mpu.getFIFOBytes(fifoBuffer, fifoCount);

			mpuIntStatus = m_mpu.getIntStatus();

			for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++)
				dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
			m_mpu.writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0],
					dmpUpdate[1]);

			m_mpu.setDMPEnabled(false);

			m_mpu.resetFIFO();
			m_mpu.getIntStatus();
		}
		else
		{
			return 2; // configuration block loading failed
		}
	}
	else
	{
		return 1; // main binary block loading failed
	}
	return 0; // success
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
		else if (m_ypr[1] < 0) angle = -180 - angle;
	}
}

/*
 * Returns the global MPU6050Manager instance
 */
MPU6050Manager* MPU6050Manager::getInstance()
{
	if (m_pInstance == 0) m_pInstance = new MPU6050Manager();

	return m_pInstance;
}

/*
 * Initializes the MPU6050 with specific offsets
 */
uint8_t MPU6050Manager::initialize(int16_t xAccelOffset, int16_t yAccelOffset,
		int16_t zAccelOffset, int16_t xGyroOffset, int16_t yGyroOffset,
		int16_t zGyroOffset)
{
	#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
		Wire.begin();
		TWBR = 24;
	#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
		Fastwire::setup(400, true);
	#endif

	m_mpu.initialize();

	Serial.println(
			m_mpu.testConnection() ?
					"MPU6050 connection successful" :
					"MPU6050 connection failed");

	Serial.println("Initializing DMP...");
	m_devStatus = dmpInitialize();

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

		m_packetSize = 42;
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
		while (m_fifoCount < m_packetSize)
			m_fifoCount = m_mpu.getFIFOCount();

		m_mpu.getFIFOBytes(m_fifoBuffer, m_packetSize);

		m_fifoCount -= m_packetSize;

		// Get quaternion values
		m_quat[0] = ((*m_pPacket[0] << 8) | *m_pPacket[1]) / 16384.0f;
		m_quat[1] = ((*m_pPacket[2] << 8) | *m_pPacket[3]) / 16384.0f;
		m_quat[2] = ((*m_pPacket[4] << 8) | *m_pPacket[5]) / 16384.0f;
		m_quat[3] = ((*m_pPacket[6] << 8) | *m_pPacket[7]) / 16384.0f;
		for (int i = 0; i < 4; i++)
			if (m_quat[i] >= 2.0f) m_quat[i] = -4.0f + m_quat[i];

		// Get gravity calculations
		m_gravity[0] = 2 * (m_quat[1] * m_quat[3] - m_quat[0] * m_quat[2]);
		m_gravity[1] = 2 * (m_quat[0] * m_quat[1] + m_quat[2] * m_quat[3]);
		m_gravity[2] = m_quat[0] * m_quat[0] - m_quat[1] * m_quat[1]
				- m_quat[2] * m_quat[2] + m_quat[3] * m_quat[3];

		// Get ypr values
		m_ypr[0] = atan2(2 * m_quat[1] * m_quat[2] - 2 * m_quat[0] * m_quat[3],
				2 * m_quat[0] * m_quat[0] + 2 * m_quat[1] * m_quat[1] - 1)
				* 180.0f / PI;
		m_ypr[1] = atan(
				m_gravity[1]
						/ sqrt(
								m_gravity[0] * m_gravity[0]
										+ m_gravity[2] * m_gravity[2]))
				* 180.0f/PI;
		m_ypr[2] = atan(
				m_gravity[0]
						/ sqrt(
								m_gravity[1] * m_gravity[1]
										+ m_gravity[2] * m_gravity[2]))
				* 180.0f/PI;

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
