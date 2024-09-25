#include <Motors.h>
#include <Sonars.h>

//#define DEBUG

Motors motorA = Motors(12, 11, 0, 50); //pwmPinA, pwmPinB, interruptNumber, directionPin
Motors motorB = Motors(10, 9, 1, 52);

Sonars sonarRF (39, 41); //Правый передний дальномер  (trigPin, echoPin)
Sonars sonarRB (43, 45); //Правый задний дальномер
Sonars sonarKR (47, 49); //Правый колдунский дальномер
Sonars sonarKL (51, 53); //Левый колдунский дальномер
Sonars sonarHR (31, 33); // сейчас на правой руке
Sonars sonarHL (35, 37); // сейчас на левой руке

uint32_t myTimer = millis(); //unsigned long
int L1, L2;
void setup()
{
  pinMode(A0, INPUT); //Датчик светоотражения
  pinMode(46, OUTPUT); //Кнопка Лего
  pinMode(48, OUTPUT); //Кнопка Лего
  digitalWrite(46, LOW); //"Кнопка Лего отпущенна"
  digitalWrite(48, LOW); //"Кнопка Лего отпущенна"

  delay(2000); //задержка
  Serial.begin(9600); //инициализиция com-порта

}

int sRF = 0; //получаем значения дальномера
int sRB = 0;
int sKR = 0;
int sKL = 0;
int sHR = 0;
int sHL = 0;
int analog0 = analogRead(A0);


//void alignToLine();

int reg(int val, int ust)
{
  const float kp = 1.2, kd = 0;
  static int errOld;
  int err = val - ust;
  int u = (err * kp) - ((errOld - err) * kd);
  errOld = err;
  return u;
}

int velocity = -128; //скорость (значения моторов от -255 до 255)
int count = 6;
float deltaL = 0;
const float smInEncoder = 0.178; // 10.2см * PI / 180 импульсов на оборот энкодера
float leftArc = 0;
float rightArc = 0;
float vel2 = 0;

void loop()
{
  int sensVal;
  int ust = 0;



  if ((count % 2) && count < 8) //езда вперед
  {
    sRF = sonarRF.value(); //получаем значения дальномера
    sHR = sonarHR.value();
    sensVal = nMotorEncoder[0] - nMotorEncoder[1];

    if ((sRF != 0 && sRF < 20) || (sHR != 0 && sHR != 4 && sHR != 5 && sHR < 20))
    {
      velocity = -128;
      count++;
      motorA.set(0);
      motorB.set(0);
      delay(1000);
      nMotorEncoder[0] = nMotorEncoder[1] = 0;
    }
  }
  else  if (!(count % 2) && count < 8) //езда назад
  {
    sRB = sonarRB.value();
    sHL = sonarHL.value();
    sensVal = nMotorEncoder[0] - nMotorEncoder[1];

    if ((sRB != 0 && sRB < 20) || (sHL != 0 && sHL < 20))
    {
      velocity = 128;
      count++;
      motorA.set(0);
      motorB.set(0);
      delay(1000);
      nMotorEncoder[0] = nMotorEncoder[1] = 0;
    }
  }
  else if (count == 8)
  {

    sKL = sonarKL.value();
    velocity = -128;
    sensVal = nMotorEncoder[0] - nMotorEncoder[1];
    ust = 0;
    if (sKL != 0 && sKL < 70) //Поменять показания на площадке!!!!!!!!!
    {
      /* motorA.set(-160);
        delay(1000);
        motorA.set(-160);
        delay(1000);*/
      count++;
      delay(200);
      motorA.set(0);
      motorB.set(0);
      delay(1000);
      sKL = sonarKL.value();
      digitalWrite(46, HIGH); //"Кнопка Лего нажата"
      delay(5000);
      nMotorEncoder[0] = nMotorEncoder[1] = 0;
      leftArc = PI * (sKL - L1) / 2;      //длина в см дуги, по которой едет левой колесо
      rightArc = PI * (sKL + L2) / 2;     //длина дуги в см, по которой едет правое колесо
      deltaL = 1.0 + (leftArc / rightArc); //отношение длинн дуг левого и правого колес
    }
  }
  else if (count == 9) //проверка в count = 8
  {
    while(nMotorEncoder[0]<leftArc&& nMotorEncoder[1]<rightArc)
    {
      delay(500);
      motorA.set(128);
      motorB.set(128);
      }
  }
  else if (count == 10) //Поворот на линии с колдуном
  {
    while (nMotorEncoder[0] < 57)
    {
      motorA.set(160);
      motorB.set(-160);
      delay(1000);
    }
    count++;
  }
  else if (count == 11)
  {
    nMotorEncoder[0] = 0;
    while (nMotorEncoder[0] < 50)// завтра надо уменьшить это расстояние, на 10 примерно,
    {
      motorA.set(-velocity);
      motorB.set(-velocity);
    }
    count++;
    velocity = -velocity;
  }
  else if (count == 12)
  {
    motorA.set(0);
    motorB.set(0);
    delay(1000);
    digitalWrite(48, HIGH); //"Кнопка Лего нажата"
    delay(5000);
  }



  motorA.set(velocity - reg(sensVal, ust)); //управление мотором А
  motorB.set(velocity + reg(sensVal, ust)); //управление мотором В

#ifdef DEBUG
  if (millis() - myTimer > 500) //millis() - возвращает количество миллисекунд с момента запуска программы
  {
    Serial.print("  count = ");
    Serial.print(count);
    Serial.print("  ust = ");
    Serial.print(ust);
    Serial.print("  sensVal = ");
    Serial.print(sensVal);
    Serial.print("  sRF = ");
    Serial.print(sRF);
    Serial.print("  sRB = ");
    Serial.print(sRB);
    Serial.print("  sKL = ");
    Serial.print(sKL);
    Serial.print("  arcLength = ");
    Serial.print(arcLength);
    Serial.print("  arcLength / smInEncoder = ");
    Serial.print(arcLength / smInEncoder);
    Serial.print("  nMotorEncoder[0] = ");
    Serial.print(nMotorEncoder[0]);
    Serial.print("  nMotorEncoder[1] = ");
    Serial.print(nMotorEncoder[1]); // вывод в com порт с переносом строки
    Serial.print("  deltaL = ");
    Serial.println(deltaL); // вывод в com порт с переносом строки
    myTimer = millis();
  }
#endif

  delay(1);
}
