/*
 * ESC.h
 *
 *  Created on: May 4, 2015
 *      Author: Mackinnon Buck
 */

#ifndef ESC_H_
#define ESC_H_

#define ESC_FREQ_MIN	1000
#define ESC_FREQ_MAX	2000

#include "Servo.h"

class ESC
{
  private:

    Servo m_servo;

    float m_value;

  public:

    ESC();
    ESC(int pin);

    void setValue(float value);

    float getValue();

};

#endif /* ESC_H_ */
