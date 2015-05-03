/*
 * ReceiverManager.h
 *
 *  Created on: May 2, 2015
 *      Author: Buck
 */

#ifndef RECEIVERMANAGER_H_
#define RECEIVERMANAGER_H_

#define PIN_COUNT 		4

#define RC_THROTTLE 	6
#define RC_YAW 			7
#define RC_PITCH 		5
#define RC_ROLL 		4

#define FREQ_MIN		1000
#define	FREQ_MAX		2000
#define FREQ_PADDING	200

#include <Arduino.h>

class ReceiverManager
{
private:

	static ReceiverManager* m_pInstance;

	uint8_t m_pins[PIN_COUNT];
	int m_time[PIN_COUNT];
	float m_values[PIN_COUNT];

	uint8_t m_currentPin;

	ReceiverManager();

	static void rise();
	static void fall();

public:

	static ReceiverManager* getInstance();

	void initialize();
	void update();

	float* getValues();

};

#endif /* RECEIVERMANAGER_H_ */
