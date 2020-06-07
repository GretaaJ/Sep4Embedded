#pragma once

typedef struct Servo Servo;
typedef struct Servo* Servo_t;


Servo_t createServo();
void openWindowServo(Servo_t self);
void closeWindowServo(Servo_t self);
void halfOpenWindowServo(Servo_t self);
