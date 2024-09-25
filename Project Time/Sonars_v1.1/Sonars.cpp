/*
 *	Библиотека для работы с ультразвуковыми дальномерами проекта Time
 *	v 1.1
 */

#include <Sonars.h>
#include <Arduino.h>
#include <wiring_private.h>

Sonars::Sonars(uint8_t trigPin, uint8_t echoPin)
{
	_trigPin = trigPin;
	_echoPin = echoPin;
	_echoBit = digitalPinToBitMask(echoPin);
	_echoPort = digitalPinToPort(echoPin);
	pinMode(_trigPin, OUTPUT);
	pinMode(_echoPin, INPUT);
	digitalWrite(_trigPin, LOW);
}

/*
uint32_t Sonars::raw()
{
	digitalWrite(_trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(_trigPin, LOW);
	return pulseIn(_echoPin, HIGH, 30000);
}
*/

uint32_t Sonars::raw()
{
	digitalWrite(_trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(_trigPin, LOW);
	
	uint32_t maxloops = microsecondsToClockCycles(30000)/16;

	uint32_t width = countPulseASM(portInputRegister(_echoPort), _echoBit, _echoBit, maxloops);

	if (width)
		return clockCyclesToMicroseconds(width * 16 + 16);
	else
		return 0;
}

uint16_t Sonars::value()
{
	return raw()/58;
}
