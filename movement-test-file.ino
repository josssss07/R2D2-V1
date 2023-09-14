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