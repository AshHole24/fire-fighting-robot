#include <Servo.h>

// ---------------- PIN DEFINITIONS ----------------
#define FLAME_SENSOR_LEFT A0
#define FLAME_SENSOR_CENTER A1
#define FLAME_SENSOR_RIGHT A2

#define MOTOR_L1 5
#define MOTOR_L2 6
#define MOTOR_R1 9
#define MOTOR_R2 10

#define RELAY_PIN 7
#define SERVO_PIN 3 

Servo waterServo;

#define FIRE_DETECTED_THRESHOLD 800

void setup() {

  Serial.begin(9600);

  pinMode(MOTOR_L1, OUTPUT);
  pinMode(MOTOR_L2, OUTPUT);
  pinMode(MOTOR_R1, OUTPUT);
  pinMode(MOTOR_R2, OUTPUT);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);   // Relay OFF (Active LOW)

  waterServo.attach(SERVO_PIN);
  waterServo.write(90);

  Serial.println("?? Robot Ready");
}

void loop() {

  int flameLeft = analogRead(FLAME_SENSOR_LEFT);
  int flameCenter = analogRead(FLAME_SENSOR_CENTER);
  int flameRight = analogRead(FLAME_SENSOR_RIGHT);

  Serial.print("L:");
  Serial.print(flameLeft);
  Serial.print(" C:");
  Serial.print(flameCenter);
  Serial.print(" R:");
  Serial.println(flameRight);

  delay(100);

  if (flameCenter < FIRE_DETECTED_THRESHOLD) {

    Serial.println("?? Fire Center");
    moveForward();
    delay(1000);

    stopMotors();
    activateWaterPump();
  }
  else if (flameLeft < FIRE_DETECTED_THRESHOLD) {

    Serial.println("?? Fire Left");
    turnLeft();
    delay(1000);
    
    stopMotors();
    activateWaterPump();
  }
  else if (flameRight < FIRE_DETECTED_THRESHOLD) {

    Serial.println("?? Fire Right");
    turnRight();
    delay(1000);
    
    stopMotors();
    activateWaterPump();
  }
  else {

    stopMotors();
    deactivateWaterPump();
  }
}

// ---------------- MOTOR FUNCTIONS ----------------

void moveForward() {

  digitalWrite(MOTOR_L1, HIGH);
  digitalWrite(MOTOR_L2, LOW);
  digitalWrite(MOTOR_R1, HIGH);
  digitalWrite(MOTOR_R2, LOW);
}

void turnLeft() {

  digitalWrite(MOTOR_L1, LOW);
  digitalWrite(MOTOR_L2, LOW);    // Left motor stopped
  digitalWrite(MOTOR_R1, HIGH);
  digitalWrite(MOTOR_R2, LOW);
}

void turnRight() {

  digitalWrite(MOTOR_L1, HIGH);
  digitalWrite(MOTOR_L2, LOW);
  digitalWrite(MOTOR_R1, LOW);
  digitalWrite(MOTOR_R2, LOW);    // Right motor stopped
}

void stopMotors() {

  digitalWrite(MOTOR_L1, LOW);
  digitalWrite(MOTOR_L2, LOW);
  digitalWrite(MOTOR_R1, LOW);
  digitalWrite(MOTOR_R2, LOW);
}

// ---------------- WATER SYSTEM ----------------

void activateWaterPump() {

  digitalWrite(RELAY_PIN, LOW);   // Pump ON (Active LOW)

  for (int pos = 0; pos <= 180; pos += 5) {
    waterServo.write(pos);
    delay(60);
  }

  for (int pos = 180; pos >= 0; pos -= 5) {
    waterServo.write(pos);
    delay(60);
  }
}

void deactivateWaterPump() {

  digitalWrite(RELAY_PIN, HIGH);  // Pump OFF
  waterServo.write(90);
