#include <Stepper.h>

const int ledPin = 13;
const int hallPin = 7;
volatile bool startFound = false;
String flap [] = {":)", ":(", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "!", "$", "%", "&", "*", "(", ")", "?", "+", "=", "/", "-"};

const int stepsPerIndex = 32; // defines the number of steps per index/character in our steppermotor setup. So moving from character 1 to 2 would be stepsPerIndex steps.
const int stepsPerRevolution = 400; // This is the steps per revolution
int currStep = 0; // tracks the current location of our stepper. we have to make a variable because Stepper.h library does not automatically track this.
int currFlap = 0; 

// these flags let us know when we have executed a given action. Used in loop() so that we don't keep moving forever.
bool calibrated = false;

Stepper stepper(stepsPerRevolution, 2, 3);

void setup() 
{
  
  Serial.begin(9600);
  pinMode(hallPin,INPUT_PULLUP); // original code used INPUT as 2nd parameter
  stepper.setSpeed(120.0); // constant speed for stepper.

}

void loop() 
{
  // calibrates the setup once by moving the stepper until the hall sensor detects the start position
  if (!calibrated) {
    calibrate();
  }
  delay(2000);

  String message [] = {"G", "O", "B", "R", "U", "N", "O", "!"}; // message for flaps to read read 
  for (int s=0; s<sizeof(message); s++) {
    moveToTarget(message[s]);
    delay(2000);
  } 
}


void calibrate() { 
  while (digitalRead(hallPin) == 1) {
    stepper.step(-1); // keep moving 1 step at a time until sensor detects start position
  }
  calibrated = true;
}
  
void moveToTarget(String target) { 
    int diff = 0; // defines the number of steps we need
    int targetFlap = 0; // defines index of target flap

    for (int i = 0; i < 50; i++) {
      if (flap[i].equals(target)) {
        targetFlap = i; 
      }
    }
    
    if (currFlap < targetFlap) {   // if current flap is earlier in array than target 
      diff = targetFlap - currFlap; // store difference 
    
    } else if (currFlap > targetFlap) { // if current flap is later in array than target
      diff = 50 - currFlap + targetFlap;
       
    } else { // letters are the same
      diff = 50; 
    }

    // resets currStep to our new location
    currFlap = targetFlap; 
    
    stepper.step(-diff*stepsPerIndex); // moved backwards --> needed to make steps a negative 
}
