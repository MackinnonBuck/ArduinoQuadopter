#include "ArduinoQuadcopter.h"

void setup()
{
	MPU6050Manager::getInstance()->initialize();
}

void loop()
{
	if (!MPU6050Manager::getInstance()->isDmpReady()) return;

	while (!MPU6050Manager::getInstance()->hasInterrupted())
	{
		// TODO: Other program logic here
	}

	MPU6050Manager::getInstance()->update();
}
