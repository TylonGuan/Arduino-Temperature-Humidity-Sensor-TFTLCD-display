#include <SoftwareSerial.h>
#include <Servo.h>
//SoftwareSerial Serial1 (2,3);
#define LED 13

// define servos
Servo myservo1;    // Servo 1 object
int pos = 0;      // variable for servo position

// define  pins
#define MOTOR2_FWD 7
#define MOTOR2_REV 8

#define MOTOR1_FWD 2
#define MOTOR1_REV 4

#define MOTOR1_ENABLE 9 // enable 1 and 2
#define MOTOR2_ENABLE 10 // enable 3 and 4

#define POWER_MAX 255
double powerLevel = 1.0;  // power level (0-1) of motors

void setup() {
  pinMode(LED, 1);
  digitalWrite(LED, 0);

  // initialize serial comms
  Serial.begin(38400);
  Serial1.begin(38400);
    Serial1.setTimeout(1000000);

  // initialize motors
  pinMode(MOTOR1_ENABLE,OUTPUT);
  pinMode(MOTOR2_ENABLE,OUTPUT);
  pinMode(MOTOR1_FWD,OUTPUT);
  pinMode(MOTOR1_REV,OUTPUT);
  pinMode(MOTOR2_FWD,OUTPUT);
  pinMode(MOTOR2_REV,OUTPUT);

  // attach servos

  //Clockwise: pos variable decrements value
//  //Counterclockwise: pos variable increments value
//  myservo1.attach(5);
//  // Initialize servos to position 0
//  myservo1.write(pos);
////  delay(1000);
//
//    digitalWrite(MOTOR1_ENABLE, HIGH);
//  digitalWrite(MOTOR2_ENABLE, HIGH);

}

byte data;

void loop() {

  // enable motors at full power


  // Receive character from bluetooth module
//  delay(10);
//  data = '?';
//  while (Serial1.available() > 0){
//    data = (char) Serial1.read();
//    Serial.print("data: ");
//    Serial.println(data);
//  
////  else
////    data = '?';
////    data = (char) Serial1.read();
//
//  // print character
//
//
//  // if joystick is tilted forward, i.e. signal to move forward
//  if (data == 'F'){
//    // drive forward
//    Serial.println("forward");
//    driveFwd();
//  }
//  // if joystick is tilted backward, i.e. signal to move backward
//  else if (data == 'B'){
//    // drive backwrad
//    Serial.println("backward");
//    driveBack();
//  }
//  // if joystick is tilted right, i.e. signal to turn right
//  else if (data == 'R') {
//    // turn right
//    Serial.println("right");
//    driveRight();
//  }
//  // if joystick is tilted left
//  else if (data == 'L') {
//    // turn left
//    Serial.println("left");
//    driveLeft();
//  }
//  delay(5);

  data = 0;   
//  Serial.print("data: ");
//  Serial.println(data);
  
  if (Serial1.available()){
//    data = (char) Serial1.read();
    data = Serial1.read();
    Serial1.flush();
    Serial.flush();
    Serial.print("data: ");
    Serial.println(data);


  }
  
  // check button press signal
  if (data == 1) {
    //Turn on the blue LED
    digitalWrite(LED, HIGH);

    // Extend the servo if it's around position 0
//    if (pos <=5){
//      // extend the servos
//      for (pos = 0; pos <= 180; pos+=5) { // goes from 0 to 180 degrees
//        // in steps of 5 degree
//        myservo1.write(pos);  // tell servo 1 to go to position
////        delay(50);        // wait 20ms for servo to reach position
//      }
//    }
//    // Retract the servo if it's around position 180
//    else if (pos >= 175){
//      // retract the servos
//      for (pos = 180; pos >= 0; pos-=5) { // goes from 180 to 0 degrees
//      // in steps of 5 degree
//        myservo1.write(pos);  // tell servo 1 to go to position
////        delay(50);        // wait 20ms for servo to reach position
//      }
//    }
  }
  else {
    //Blue LED is off
    digitalWrite(LED, LOW);
  }
//  delay(10);
}


void driveFwd() {
  digitalWrite(MOTOR1_FWD, HIGH); // motor 1 forward
  digitalWrite(MOTOR1_REV, LOW);
  digitalWrite(MOTOR2_FWD, HIGH); // motor 2 forward
  digitalWrite(MOTOR2_REV, LOW);
}

void driveBack() {
  digitalWrite(MOTOR1_FWD, LOW);  // motor 1 reverse
  digitalWrite(MOTOR1_REV, HIGH);
  digitalWrite(MOTOR2_FWD, LOW);  // motor 2 reverse
  digitalWrite(MOTOR2_REV, HIGH);
}

void driveLeft() {
  digitalWrite(MOTOR1_FWD, LOW);  // motor 1 reverse
  digitalWrite(MOTOR1_REV, HIGH);
  digitalWrite(MOTOR2_FWD, HIGH); // motor 2 forward
  digitalWrite(MOTOR2_REV, LOW);
}
void driveRight() {
  digitalWrite(MOTOR1_FWD, HIGH); // motor 1 forward
  digitalWrite(MOTOR1_REV, LOW);
  digitalWrite(MOTOR2_FWD, LOW);  // motor 2 reverse
  digitalWrite(MOTOR2_REV, HIGH);
}
void driveStop() {
  digitalWrite(MOTOR1_FWD, LOW);
  digitalWrite(MOTOR1_REV, LOW);
  digitalWrite(MOTOR2_FWD, LOW);
  digitalWrite(MOTOR2_REV, LOW);
}
