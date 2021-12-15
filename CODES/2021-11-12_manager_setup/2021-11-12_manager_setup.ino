#include <SoftwareSerial.h>
SoftwareSerial Serial1 (2, 3); // Rx | Tx
#define LED 11
#define PRESSED LOW

int input;
char val; //data to send

// joystick pins
int VRx = A0; // analog potentiometers
int VRy = A1;
int SW = 4; // joystick built-in button

// position of joystick and state of button
int xPosition = 0;
int yPosition = 0;
int SW_state = 0;
int mapX = 0;
int mapY = 0;

// character to know that the button was pressed
char signalPress = 'P';

void setup() 
{
  //Serial.begin(9600); 
  
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP); 
  

  pinMode(LED, OUTPUT);
//  pinMode(PB_pin, INPUT_PULLUP);
  Serial.begin(38400);  // Baud rate
  Serial1.begin(38400);
  //HC05.begin(38400);
  Serial.setTimeout(1000000);

}


void loop() {

  // read stick position values
  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  // read push button state
  SW_state = digitalRead(SW);

  // map stick values
  mapX = map(xPosition, 0, 1023, -512, 512);
  mapY = map(yPosition, 0, 1023, 512, -512);

    if(SW_state == 0){
      byte one = 1;
    Serial1.write(one); // P for press
    Serial.println(one);
    Serial.flush();
    Serial1.flush();
    delay(200);

//    delay(100);   //0.1 second
  }
  

  // print stick values and button state
//  Serial.print("X: ");
//  Serial.print(mapX);
//  Serial.print(" | Y: ");
//  Serial.print(mapY);
//  Serial.print(" | Button: ");
//  Serial.println(SW_state);


  

  //input = digitalRead(PB_pin);  // Read pushbutton

  // drive forward, backward, left, or right depending on analog stick reading
  // by broadcasting Serial1 signal
//  delay(50);
  // if stick is in positive y, drive forward
//  if(mapY >= 150){
//    Serial1.write('F'); // F for forward
//    Serial.println('F');
//    //val = '1';
//  }
//  // if stick is in negative y, drive backward
//  else if (mapY <=-150) {
//    Serial1.write('B'); // B for backward
//    Serial.println('B');
//   // val = '0';
//  }
//  // if stick is in neutral y, look at x instead
//  else {
//    // if stick is in positive x, turn right
//    if (mapX >= 150) {
//      Serial1.write('R'); // R for right
//      Serial.println('R');
//    }
//    // if stick is in negative x, turn left
//    else if (mapX <= -150) {
//      Serial1.write('L'); // L for left
//      Serial.println('L');
//    }
//    else{
//      // Serial 1 shouldn't write anything. Stop the robot's movement
////      Serial1.write('N');
////      //Serial.println('N');
//    }
//  }
//  Serial1.flush();

  // check button press

}
