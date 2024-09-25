/*
 *	Библиотека для работы с ультразвуковыми дальномерами проекта Time
 *	v 1.1
 */
 
#include <Sonars.h>

Sonars sonar1 (4, 3); // Создаём экземпляр класса Sonars (trigPin, echoPin)

uint32_t myTimer = millis(); // Создаём "таймер", millis() - возвращает количество миллисекунд с момента начала выполнения текущей программы

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if(millis() - myTimer > 500) 
  {
	Serial.print("Sonar.raw() = ");
    Serial.print(sonar1.raw()); //метод raw() - возвращает показания датчика в микросекундах (unsigned long)
    
	Serial.print("   Sonar.value() = ");
    Serial.println(sonar1.value()); //метод value() - возвращает показания датчика в сантиметрах (unsigned int)
	
	myTimer = millis();
  }
}
