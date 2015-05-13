/*
 * ReceiverManager.h
 *
 *  Created on: May 2, 2015
 *      Author: Buck
 */

#ifndef RECEIVERMANAGER_H_
#define RECEIVERMANAGER_H_

#define PIN_COUNT 					4

#define RC_THROTTLE_PIN 			6
#define RC_YAW_PIN 					7
#define RC_PITCH_PIN 				5
#define RC_ROLL_PIN 				4

#define RC_THROTTLE_DEFAULT 		0
#define RC_YAW_DEFAULT				0.5
#define RC_PITCH_DEFAULT			0.5
#define RC_ROLL_DEFAULT				0.5

#define RECEIVER_FREQ_MIN			1000
#define	RECEIVER_FREQ_MAX			2000

#define RECEIVER_UPDATE_TIMEOUT		100000

#include <Arduino.h>

class ReceiverManager
{
private:

	static ReceiverManager* m_pInstance;

	uint8_t m_pins[PIN_COUNT];
	float m_values[PIN_COUNT];
	int m_time[PIN_COUNT];

	uint8_t m_currentPin;

	long m_startTime;
	long m_endTime;

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
