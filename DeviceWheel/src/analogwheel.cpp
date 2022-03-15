#include "analogwheel.h"

#if defined(ESP32)
const int motorChannelA = 0;
const int motorChannelB = 1;
#endif

void AnalogWheel::init(uint8_t _motorPinA, uint8_t _motorPinB)
{
    motorPinA = _motorPinA;
    motorPinB = _motorPinB;
    
    pinMode(motorPinA, OUTPUT);
    pinMode(motorPinB, OUTPUT);

#if defined(ESP32)
  ledcSetup(motorChannelA, 1000, 8);
  ledcSetup(motorChannelB, 1000, 8);
  ledcAttachPin(motorPinA, motorChannelA);
  ledcAttachPin(motorPinB, motorChannelB);
#endif

}

void AnalogWheel::set(int v)
{
  v = constrain(v, -1024, 1024);

  float valueA = 0;
  float valueB = 0;

  if (v != 0)
  {
    if (v > 0)
    {
      valueA = v;
      valueB = 0;

      digitalWrite(motorPinA, HIGH);
      digitalWrite(motorPinB, LOW);
    }
    else
    {
      valueA = 0;
      valueB = abs(v);

      digitalWrite(motorPinA, LOW);
      digitalWrite(motorPinB, HIGH);
    }
    delay(25);
  }

#if defined(ESP32)
  ledcWrite(motorChannelA, valueA);
  ledcWrite(motorChannelB, valueB);
#else
  analogWrite(motorPinA, valueA);
  analogWrite(motorPinB, valueB);
#endif
}