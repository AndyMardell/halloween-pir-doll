
//-----------------------------------------------------------------
//  Include Libraries
//-----------------------------------------------------------------

#include <Servo.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

//-----------------------------------------------------------------
//  Setup Servo
//-----------------------------------------------------------------

Servo headservo;
int servoPin = 9;
int pos = 0; // Servo Position

//-----------------------------------------------------------------
//  Setup DfPlayer
//-----------------------------------------------------------------

SoftwareSerial softwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini dfplayer;
int totalTracks = 5; // Total number of tracks on player

//---------------------------------------
// Declare Other Vars
//---------------------------------------

int pirPin = 2;
int pirState = LOW;
int pirStatus = 0;

int pauseTime = 5000;
int triggered = 0; // Times triggered

//---------------------------------------
// Setup
//---------------------------------------

void setup() {

  // Setup Software Serial for DfPlayer
  softwareSerial.begin(9600);

  // Setup Serial
  Serial.begin(115200);

  // Declare PIR as input
  pinMode(pirPin, INPUT);

  // Attach servo
  headservo.attach(servoPin);

  // Check softwareSerial can communicate with mp3.
  if (!dfplayer.begin(softwareSerial)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection"));
    Serial.println(F("2.Please insert the SD card"));
    while(true);
  }
  Serial.println(F("DFPlayer online"));

  // Set volume to 10
  dfplayer.volume(10);

}

//---------------------------------------
// Loop
//---------------------------------------
 
void loop(){

  // Check PIR status
  pirStatus = digitalRead(pirPin);

  // If PIR is sensing motion
  if (pirStatus == HIGH) {
    
    // Increment triggered
    triggered++;

    // If state is set to low
    if (pirState == LOW) {
      Serial.println("Motion detected");
      // Change state to high (sensing motion)
      pirState = HIGH;
    }

    // Play sound
    playTrack(totalTracks);

    // If triggered 5 times
    if (triggered > 5) {
      // Move head 180º, fast
      moveHead(180, 5);
      // Reset count
      triggered = 0;
    } else {
      // Move head 90º, slow
      moveHead(90, 15);
    }

    // Don't trigger again for pauseTime
    delay(pauseTime);

  // If PIR is not sensing motion
  } else {

    // Do nothing

    // If state is set to high
    if (pirState == HIGH){
      Serial.println("Motion ended");
      // Change state to low
      pirState = LOW;
    }

  }

}

//-----------------------------------------------------------------
//  Move Head
//-----------------------------------------------------------------

void moveHead(int rotateTo, int speed) {

  // Move head to rotateTo in 1º steps
  for (pos = 0; pos <= rotateTo; pos += 1) {
    headservo.write(pos);
    delay(speed);
  }

  // Twitch
  twitch(rotateTo);

  // Move head back to 0
  for (pos = rotateTo; pos >= 0; pos -= 1) {
    headservo.write(pos);
    delay(speed);
  }

}

//-----------------------------------------------------------------
//  Twitch
//-----------------------------------------------------------------

void twitch(int currentPos) {

  // Set twitchpos as 5º less than current position
  int twitchPos = currentPos - 5;

  // Twitch back slow
  for (pos = currentPos; pos >= twitchPos; pos -= 1) {
    headservo.write(pos);
    delay(15);
  }
  // Twitch forward fast
  for (pos = twitchPos; pos <= currentPos; pos += 1) {
    headservo.write(pos);
    delay(2);
  }

}

//-----------------------------------------------------------------
//  Play Random Sound
//-----------------------------------------------------------------

void playTrack(int tracks) {

  // random() is exclusive of max number. 
  // 10 would be 1-9, we want it 1-10, so add 1
  int max = tracks + 1;

  // Get number between 0 and max
  int val = random(1, max);

  // Play random track
  dfplayer.play(val);

}
