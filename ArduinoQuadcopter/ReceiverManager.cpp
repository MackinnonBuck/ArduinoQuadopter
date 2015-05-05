/*
 * ReceiverManager.cpp
 *
 *  Created on: May 2, 2015
 *      Author: Buck
 */

#include "ReceiverManager.h"
#include "EnableInterrupt.h"

ReceiverManager* ReceiverManager::m_pInstance = 0;

ReceiverManager::ReceiverManager() :
		m_currentPin(0), m_startTime(0), m_endTime(0)
{
	m_pins[0] = RC_THROTTLE;
	m_pins[1] = RC_YAW;
	m_pins[2] = RC_PITCH;
	m_pins[3] = RC_ROLL;

	for (int i = 0; i < PIN_COUNT; i++)
	{
		m_time[i] = 0;
		m_values[i] = 0;
	}
}

ReceiverManager* ReceiverManager::getInstance()
{
	if (m_pInstance == 0) m_pInstance = new ReceiverManager();

	return m_pInstance;
}

void ReceiverManager::rise()
{
	getInstance()->m_startTime = micros();
	disableInterrupt(getInstance()->m_pins[getInstance()->m_currentPin]);
	enableInterrupt(getInstance()->m_pins[getInstance()->m_currentPin], fall, FALLING);
}

void ReceiverManager::fall()
{
	getInstance()->m_endTime = micros();
	getInstance()->m_time[getInstance()->m_currentPin] = getInstance()->m_endTime - getInstance()->m_startTime;
	disableInterrupt(getInstance()->m_pins[getInstance()->m_currentPin]);
	getInstance()->m_currentPin++;
	getInstance()->m_currentPin = getInstance()->m_currentPin % PIN_COUNT;
	enableInterrupt(getInstance()->m_pins[getInstance()->m_currentPin], rise, RISING);
}

void ReceiverManager::initialize()
{
	for (int i = 0; i < PIN_COUNT; i++)
	{
		pinMode(m_pins[i], INPUT_PULLUP);
	}

	enableInterrupt(m_pins[m_currentPin], rise, RISING);
}

void ReceiverManager::update()
{
	for (int i = 0; i < PIN_COUNT; i++)
	{
		m_values[i] = (float)(m_time[i] - RECEIVER_FREQ_MIN) / (float)(RECEIVER_FREQ_MAX - RECEIVER_FREQ_MIN);
		if (m_values[i] < 0.0)
			m_values[i] = 0.0;
		else if (m_values[i] > 1.0)
			m_values[i] = 1.0;
	}
}

float* ReceiverManager::getValues()
{
	return m_values;
}
