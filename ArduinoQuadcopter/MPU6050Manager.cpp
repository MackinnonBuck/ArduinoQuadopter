/*
 * MPU6050Manager.cpp
 *
 *  Created on: Apr 19, 2015
 *      Author: Mackinnon Buck
 */

#include "MPU6050Manager.h"

MPU6050Manager* MPU6050Manager::m_pInstance = 0;

/*
 * Default constructor
 */
MPU6050Manager::MPU6050Manager() :
  m_initSuccessful(false), m_updateTime(0), m_deltaTime(0)
{
  for (int i = 0; i < 3; i++)
  {
    m_accel[i] = 0;
    m_gyro[i] = 0;
    m_rate[i] = 0.0f;
    m_gravity[i] = 0.0f;
    m_ypr[i] = 0.0f;
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
bool MPU6050Manager::initialize(int16_t xAccelOffset, int16_t yAccelOffset,
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
    m_initSuccessful = m_mpu.testConnection() ?
    "MPU6050 connection successful" :
    "MPU6050 connection failed");

  if (m_initSuccessful)
  {
    m_mpu.setXAccelOffset(xAccelOffset);
    m_mpu.setYAccelOffset(yAccelOffset);
    m_mpu.setZAccelOffset(zAccelOffset);
    m_mpu.setXGyroOffset(xGyroOffset);
    m_mpu.setYGyroOffset(yGyroOffset);
    m_mpu.setZGyroOffset(zGyroOffset);
    
    m_mpu.setFullScaleAccelRange(3); // 16g max
    m_mpu.setFullScaleGyroRange(3); // 2000 dps max
    
    update();
  }

  return m_initSuccessful;
}

/*
 * Returns true if initialization succeeded
 */
bool MPU6050Manager::initSuccessful()
{
  return m_initSuccessful;
}

/*
 * Updates the accel, gyro, gravity, rate and ypr values
 */
void MPU6050Manager::update()
{
  if (m_initSuccessful)
  {
    m_currentTime = millis();
    m_deltaTime = m_currentTime - m_updateTime;
    m_updateTime = m_currentTime;
    
    m_mpu.getMotion6(&m_accel[0], &m_accel[1], &m_accel[2], &m_gyro[0], &m_gyro[1], &m_gyro[2]);
    
    m_gravity[0] = (float)m_accel[0] * (ACCEL_RANGE / 32768.0f);
    m_gravity[1] = (float)m_accel[1] * (ACCEL_RANGE / 32768.0f);
    m_gravity[2] = (float)m_accel[2] * (ACCEL_RANGE / 32768.0f);
    
    m_rate[0] = (float)m_gyro[0] * (GYRO_RANGE / 32768.0f);
    m_rate[1] = -(float)m_gyro[1] * (GYRO_RANGE / 32768.0f);
    m_rate[2] = (float)m_gyro[2] * (GYRO_RANGE / 32768.0f);
    
    m_ypr[0] += m_rate[2] * (m_deltaTime / 1000.0f);
    m_ypr[1] = MPU6050_FILTER * (m_ypr[1] + m_rate[0] * (m_deltaTime / 1000.0f)) + (1 - MPU6050_FILTER) * (atan(m_gravity[1] / sqrt(sq(m_gravity[0]) + sq(m_gravity[2]))) * RAD_TO_DEG);
    m_ypr[2] = MPU6050_FILTER * (m_ypr[2] + m_rate[1] * (m_deltaTime / 1000.0f)) + (1 - MPU6050_FILTER) * (atan(m_gravity[0] / sqrt(sq(m_gravity[1]) + sq(m_gravity[2]))) * RAD_TO_DEG);
  }
}

/*
 * Returns the accel values
 */
int16_t* MPU6050Manager::getAccel()
{
  return m_accel;
}

/*
 * Returns the gyro values
 */
int16_t* MPU6050Manager::getGyro()
{
  return m_gyro;
}

/*
 * Returns the rate values
 */
float* MPU6050Manager::getRate()
{
  return m_rate;
}

/*
 * Returns the gravity values
 */
float* MPU6050Manager::getGravity()
{
  return m_gravity;
}

/*
 * Returns the ypr values
 */
 float* MPU6050Manager::getYawPitchRoll()
{
  return m_ypr;
}
