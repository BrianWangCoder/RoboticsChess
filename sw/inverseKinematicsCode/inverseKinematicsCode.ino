#include <Servo.h>
#include <math.h>

//Servo Pins
#define SERVO_BASE_PORT      2
#define SERVO_ELBOW_PORT     3
#define SERVO_WRIST_PORT     4
#define SERVO_GRIPPER_PORT   5  

//Stepper motor pins
#define DIR           13
#define STEP          12

Servo servoBase;
Servo servoElbow;
Servo servoWrist;
Servo servoGripper;

float L1 = 145.0;
float L2 = 154.0;
float L3 = 127.0;

float offset1 = 0;
float offset2 = 47;
float offset3 = 100;

void setup() {
  Serial.begin(115200);
  servoBase.attach(SERVO_BASE_PORT);
  servoElbow.attach(SERVO_ELBOW_PORT);
  servoWrist.attach(SERVO_WRIST_PORT);
  servoGripper.attach(SERVO_GRIPPER_PORT);

  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  gripperGrab(true);
  moveTo(80, -30, -90);
  delay(2000);
  moveTo(110, -30, -90);
  delay(2000);
  
}
void moveTo(float j, float k, float alpha) {

  float n = k - L3 * sin(radians(alpha));
  float m = j - L3 * cos(radians(alpha));

  float l = sqrt(m*m + n*n);

  float theta_112 = degrees(atan2(n,m));

  float val_13 = (l*l + L1*L1 - L2*L2) / (2*l*L1);
  val_13 = constrain(val_13, -1.0, 1.0);
  float theta13 = degrees(acos(val_13));
  float value_phi_2 = (l*l - L2*L2 - L1*L1)/(-2 * L2 * L1);

  float deg1 = theta_112 + theta13;
  float deg2 = degrees(acos(value_phi_2)) - offset2;
  float theta_2 = deg1 + deg2 - 180;
  float deg3 = (180 - theta_2) + alpha - offset3;

  
  // ---- Move servos ----
  servoBase.write(deg1);
  servoElbow.write(deg2);
  servoWrist.write(map(deg3,0,214,0,180));

  // ---- Debug ----
  Serial.print("Move to: ");
  Serial.print(j); Serial.print(", ");
  Serial.print(k); Serial.print(" | ");

  Serial.print("Angles: ");
  Serial.print(deg1); Serial.print(", ");
  Serial.print(deg2); Serial.print(", ");
  Serial.println(deg3);
}
void gripperGrab(bool isGrab){
  if(isGrab) servoGripper.write(0);
  else servoGripper.write(90);
}
void stepMotor(int steps, bool direction,int speed) {
  digitalWrite(DIR, direction);

  for(int i = 0; i < steps; i++) {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(speed);  // speed control
    digitalWrite(STEP, LOW);
    delayMicroseconds(speed);
  }
}
void loop() {
  
}
