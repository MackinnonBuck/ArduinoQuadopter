/*
 * ESC.cpp
 *
 *  Created on: May 4, 2015
 *      Author: Buck
 */

#include "ESC.h"

ESC::ESC() : m_value(0.0f)
{
}

ESC::ESC(int pin) :
		m_value(0.0f)
{
	m_servo.attach(pin);
}

void ESC::setValue(float value)
{
	m_value = value;
	m_servo.writeMicroseconds((m_value * ESC_FREQ_MIN) + (ESC_FREQ_MAX - ESC_FREQ_MIN));
}

float ESC::getValue()
{
	return m_value;
}
