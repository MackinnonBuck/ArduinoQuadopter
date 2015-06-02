/*
 * MPU6050Manager.h
 *
 *  Created on: Apr 19, 2015
 *      Author: Mackinnon Buck
 */

#ifndef _MPU6050MANAGER_H_
#define _MPU6050MANAGER_H_

#include "MPU6050.h"

#define GYRO_RANGE            2000
#define ACCEL_RANGE           16
#define MPU6050_FILTER        0.95

class MPU6050Manager
{

  private:

    static MPU6050Manager* m_pInstance;

    MPU6050 m_mpu;

    bool m_initSuccessful;
    
    int16_t m_accel[3];
    int16_t m_gyro[3];
    
    float m_rate[3];
    float m_gravity[3];
    
    float m_ypr[3];
    
    unsigned long m_updateTime;
    unsigned long m_currentTime;
    unsigned long m_deltaTime;

    MPU6050Manager();

  public:

    static MPU6050Manager* getInstance();

    bool initialize(int16_t xAccelOffset, int16_t yAccelOffset,
                       int16_t zAccelOffset, int16_t xGyroOffset, int16_t yGyroOffset,
                       int16_t zGyroOffset);

    bool initSuccessful();
    
    int16_t* getAccel();
    int16_t* getGyro();
    
    float* getRate();
    float* getGravity();
    
    float* getYawPitchRoll();
    
    void update();
};

#endif /* _MPU6050MANAGER_H_ */
