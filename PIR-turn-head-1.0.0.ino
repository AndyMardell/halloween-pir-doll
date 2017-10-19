
//-----------------------------------------------------------------
//  Include Libraries
//-----------------------------------------------------------------

#include <Servo.h>

//-----------------------------------------------------------------
//  Setup Servo
//-----------------------------------------------------------------

Servo headservo;

//---------------------------------------
// Declare Vars
//---------------------------------------

int pirPin = 2;
int pirState = LOW;
int pirStatus = 0;

int servoPin = 9;
int pos = 0; // Servo Position

//---------------------------------------
// Setup
//---------------------------------------

void setup() {

  // Declare PIR as input
  pinMode(pirPin, INPUT);

  // Attach servo
  headservo.attach(servoPin);
 
  // Setup Serial
  Serial.begin(9600);
}

//---------------------------------------
// Loop
//---------------------------------------
 
void loop(){

  // Check PIR status
  pirStatus = digitalRead(pirPin);

  // If PIR is sensing motion
  if (pirStatus == HIGH) {
    
    // If state is set to low
    if (pirState == LOW) {
      Serial.println("Motion detected!");
      // Change state to high (sensing motion)
      pirState = HIGH;
    }

    // Move Head
    moveHead();

  // If PIR is not sensing motion
  } else {

    // Do nothing

    // If state is set to high
    if (pirState == HIGH){
      Serial.println("Motion ended!");
      // Change state to low
      pirState = LOW;
    }

  }

}

//-----------------------------------------------------------------
//  Move Head
//-----------------------------------------------------------------

void moveHead() {

  // Move head 90º right in 1º steps
  for (pos = 0; pos <= 90; pos += 1) {
    headservo.write(pos);
    delay(15);
  }

  // Move head back
  for (pos = 90; pos >= 0; pos -= 1) {
    headservo.write(pos);
    delay(15);
  }

  // Don't trigger again for a few seconds
  delay(5000);

}
