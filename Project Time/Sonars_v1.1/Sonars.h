/*
 *	Библиотека для работы с ультразвуковыми дальномерами проекта Time
 *	v 1.1
 */
 
#ifndef Sonars_h
#define Sonars_h
#include <Arduino.h>

class Sonars
{
	public:
		Sonars(uint8_t trigPin, uint8_t echoPin);
		uint16_t value();
		uint32_t raw();
	private:
		uint8_t _trigPin;
		uint8_t _echoPin;
		uint8_t _echoBit;
		uint8_t _echoPort;
};

#endif

