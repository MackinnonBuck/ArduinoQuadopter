################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ArduinoQuadcopter.cpp \
../I2Cdev.cpp \
../MPU6050.cpp \
../MPU6050Manager.cpp \
../ReceiverManager.cpp \
../TimerOne.cpp 

LINK_OBJ += \
./ArduinoQuadcopter.cpp.o \
./I2Cdev.cpp.o \
./MPU6050.cpp.o \
./MPU6050Manager.cpp.o \
./ReceiverManager.cpp.o \
./TimerOne.cpp.o 

CPP_DEPS += \
./ArduinoQuadcopter.cpp.d \
./I2Cdev.cpp.d \
./MPU6050.cpp.d \
./MPU6050Manager.cpp.d \
./ReceiverManager.cpp.d \
./TimerOne.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
ArduinoQuadcopter.cpp.o: ../ArduinoQuadcopter.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Arduino/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=155-r2 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR    -I"C:\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Arduino\hardware\arduino\avr\libraries\Wire\utility" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

I2Cdev.cpp.o: ../I2Cdev.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Arduino/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=155-r2 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR    -I"C:\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Arduino\hardware\arduino\avr\libraries\Wire\utility" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

MPU6050.cpp.o: ../MPU6050.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Arduino/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=155-r2 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR    -I"C:\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Arduino\hardware\arduino\avr\libraries\Wire\utility" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

MPU6050Manager.cpp.o: ../MPU6050Manager.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Arduino/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=155-r2 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR    -I"C:\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Arduino\hardware\arduino\avr\libraries\Wire\utility" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

ReceiverManager.cpp.o: ../ReceiverManager.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Arduino/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=155-r2 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR    -I"C:\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Arduino\hardware\arduino\avr\libraries\Wire\utility" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '

TimerOne.cpp.o: ../TimerOne.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Arduino/hardware/tools/avr/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=155-r2 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR    -I"C:\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Arduino\hardware\arduino\avr\libraries\Wire\utility" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '


