// Arduino joystick DC motor speed and rotation direction control

#define joystick     A0
#define pwm1          9
#define pwm2         10

int motor_control;

void setup() {
  pinMode(pwm1,   OUTPUT);
  pinMode(pwm2,   OUTPUT);
}

void loop() {
  motor_control = analogRead(joystick);
  motor_control >>= 1;
  if(motor_control > 255){
    digitalWrite(pwm2, 0);
    analogWrite(pwm1, (motor_control - 256));
  }
  else
    if(motor_control < 255){
      digitalWrite(pwm1, 0);
      analogWrite(pwm2, (255 - motor_control));
    }
    else{
      digitalWrite(pwm1, 0);
      digitalWrite(pwm2, 0);
    }
}




//example 2


// This program translates a value that is read from a potentiometer into a PWM value
// to drive a motor using the Pololu Simple Motor Controller.
#include <Servo.h> 

Servo motor;

// constants won't change.
#define PIN_POTENTIOMETER 2
#define PIN_PWM 9

void setup()
{motor.attach(PIN_PWM);
}

void loop()
{int potValue = analogRead(PIN_POTENTIOMETER);  // leitura do potenciômetro
 int speed = map(potValue, 0, 1023, 0, 180);
  
 if( speed==90) motor.write(LOW);
   
 delay(30); // a cada 30 ms
 motor.write(speed); // atualiza a velocidade do motor
  
 delay(300); // pausa de 300 ms
}
