#Arduino Quadcopter Project

This project contains the code for a quadcopter that I have built. Feel free to copy/modify my code to fit your own needs.

Following is a list of:
* Major hardware components, a small description of them, and software notes regarding their use
* Other minor components
* Libraries used

#Arduino Nano v3.0 (ATmega328P)
* Description: Every quadcopter needs a flight controller to process IMU inputs, receiver inputs, and flight algorithms. As you probably know from looking at the name of this project, I am using an Arduino Nano as a flight controller for my quadcopter. The Arduino Nano is essentially a scaled-down version of the Arduino Uno. It is powered by the ATmega328P microcontroller, unlike versions 2.x of the Nano. It has 14 digital IO pins (6 of which provide PWM output), 8 analog pins, and a 16 MHz clock speed. Hence its name, the Arduino Nano is only 45 x 18 mm in size and weighs only 5 grams.
* Software notes: If you decide to use an Arduino other than the Arduino Nano, you will most likely only need to change project settings and maybe some pin values.

#MPU6050 IMU
* Description: The IMU (inertial measurement unit) is used to provide input to the flight controller to make the quadcopter self-balancing. The MPU6050 is an IMU that contains a gyroscope, an accelerometer, and a digital motion processor for determining 3D angles and angular velocity.
* Softare notes: The I2CDev and MPU6050 libraries were used to program this device. Even if you plan on using an MPU6050 IMU, you will have to adjust the device offsets in the MPU6050Manager::getInstance()->initialize(...) function call in ArduinoQuadcopter.cpp to match your device. Although you can't use the MPU6050 library if you aren't using an MPU6050 IMU, the I2CDev project contains other libraries supporting other IMU devices. The I2CDev repository can be found here: https://github.com/jrowberg/i2cdevlib. Note that although my project does not contain the MPU6050 Motion Apps library for dmp access, I have extracted parts of it for use in this project (the dmp memory slots in MPU6050Manager.h and the dmpInitialize() function in MPU6050Manager.cpp were based of parts of the MPU6050 Motion Apps library). Be prepared to make major software changes to MPU6050Manager.h and MPU6050Manager.cpp if you use an IMU other than the MPU6050.

#Flysky FS-T6 transmitter
* Description: The transmitter (radio controller) is used by the pilot to send input to the quadcopter to control where it flies. The Flysky FS-T6 transmitter has six axes and runs on 2.4 GHz.
* Software notes: My transmitter is set-up for mode 2 (throttle and yaw control on left - pitch and roll on right), so you might need to change a few macros in ReceiverManager.h if you use mode 1. Also, the FlySky FS-T6 outputs pulses ranging from 1000 - 2000 microseconds in width, so you might need to change the corresponding macros in ReceiverManager.h if you use a transmitter with different output frequencies.

#Neewer 30A ESCs (4X)
* Description: ESCs (electronic speed controllers) take input signals from the flight controller and output pulses to the brushless motor to power it. ESCs are a necessity to operate brushless motors. The Neewer 30A ESC is just like any other ESC, but it is a little cheaper (and has some quality issues based on my experience).
* Software notes: No software changes should be necessary if you decide to use a different ESC than the one I am using. If you find that the motor is spinning the wrong direction, don't go hunting through the code trying to find where I programmed the motor direction (you won't find it 'cause it's not there). If you want to reverse the motor direction, switch any two of the output wires on the ESC.

#Neewer 1000KV brushless motors (4X)
* Description: Quadcopters need motors to fly. Because brushless motors have fewer parts that can wear out or break than a brushed motor, they are a popular choice for RC toys. 1000KV (RPM per volt) motors should provide enough power for flying the typical quadcopter.
* Software notes: If you decide to use different motors than the ones listed above, the only programmatical changes you might need to make are adjustments to the PID inputs.

#Neewer 10x4.5" counter-rotating propeller blades (4X)
* Description: Propellers are what provide lifting force to get the quadcopter off the ground. Counter-rotating propellers are used to prevent the quadcopter from spinning out of control.
* Software notes: Switching the propellers above with those of a different type might only require adjustments to the PID inputs.

#Neewer HJ450 Frame
* Description: The Neewer HJ450 Frame is 450mm long diagonally and has a built-in power distribution board for supplying power to the ESCs and the flight controller. The frame comes with straps for securing the battery.
* Software notes: Using a different frame than the one listed above might only require changes to the PID inputs.

#Other components (no software changes required)
* 3.5 mm bullet connectors
* Battery plug connectors
* 12 AWG red and black wire
* Heat shrink
* Jumper wires (male to female & male only)
* Receiver (mine came with my transmitter)
* 400-point breadboard
* Electrical tape (for protecting wire connections to the power distribution board)
* Masking tape (for securing the ESCs)
* Battery charger (doesn't actually go on the quadcopter)

#Libraries used
* i2cdevlib (https://github.com/jrowberg/i2cdevlib)
* EnableInterrupt (https://github.com/GreyGnome/EnableInterrupt)
* Arduino-PID-Library (https://github.com/br3ttb/Arduino-PID-Library)
* Arduino Wire library
* Arduino Servo library
