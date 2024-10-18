// Stepper Light Seeker
// By Will J. for Programming and Hardware MEGN-200 Fall 2024
// Uses a stepper motor and photoresistor to point in the direction
// where light is the brightest.

// Detailed operation:
// On button press, motor sweeps 180 degrees from its initial position, 
// taking samples of light every degree, returning to 0,
// then moving to the position with thehighest level of light,
// linger there for one second, then return to 0 again.

#include <Stepper.h>

// Pins
#define PIN_STEPPER_1 8
#define PIN_STEPPER_2 10
#define PIN_STEPPER_3 9
#define PIN_STEPPER_4 11
#define PIN_PHOTORESISTOR_0 A0
#define PIN_PHOTORESISTOR_1 A1
#define PIN_BUTTON 3

// Globals
const int stepsPerRev = 2048;
int brightness = 0;
int highestBrightness = 0;
int highestBrightnessPos = 0;
float currentAngle = 0.0;

Stepper myStepper(stepsPerRev, PIN_STEPPER_1, PIN_STEPPER_2, PIN_STEPPER_3, PIN_STEPPER_4);


void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");

  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_PHOTORESISTOR_0, INPUT);
  pinMode(PIN_PHOTORESISTOR_1, INPUT);

  myStepper.setSpeed(10); // Recommended 10, max 20, we're gonna push things a little.
  
  // Sweep the stepper for visual confirmation
  // stepperGoTo(180);
  // stepperGoTo(0);
}

void loop() {
  // On button press
  if (digitalRead(PIN_BUTTON) == LOW) {
    brightness = 0;
    highestBrightness = 0;
    highestBrightnessPos = 0;

    for (int i = 0; i <= 120; i++) {
      // Serial.println("Moving to" + (String)i);
      stepperGoTo(i);
      brightness = analogRead(PIN_PHOTORESISTOR_0);
      // Serial.println("Reading at " + (String)i + ": " + (String)brightness);

      // Set new highest brightness if need be
      if (brightness > highestBrightness) {
        highestBrightness = brightness;
        highestBrightnessPos = i;
      }
    }

    Serial.println("Moving to: " +  (String)highestBrightnessPos + " with value: " + (String)highestBrightness);
    stepperGoTo(highestBrightnessPos);

    // Then track for 10 seconds
    long trackTime = millis() + 10000;
    while (millis() < trackTime) {
      int brightDiff = analogRead(PIN_PHOTORESISTOR_0) - analogRead(PIN_PHOTORESISTOR_1);
      Serial.println((String)millis() + "   " + (String)trackTime);
      if (brightDiff > 20) {
        // Brighter on the right side
        stepperGoTo(currentAngle + 1.0);
      } else if (brightDiff < -20) {

        stepperGoTo(currentAngle - 1.0);
      }
      delay(100);
    }

    stepperGoTo(0);

  } 
  // Serial.println(analogRead(PIN_PHOTORESISTOR_0));
}

// Pretend I'm making my own proper stepper class
// define "stepsPerDegree" as an integer equal to 6 steps/angular degree, which is the approximate # of integer steps per degree of motion
// There are 5.68888 repeating (or 256/45) steps per degree. Using an integer here would quickly lead to positional innacuracies, so I've elected to bring in floats.
const float stepsPerDegree = 2048.0 / 360.0; // Every 256 steps is 45 degrees

void stepperGoTo(float newAngle) {

  // define integer steps variable as the difference between the newAngle and the current input variables multipled by the stepsPerDegree variable to convert the change in angle to number of steps
  // Difference of newAngle and CurrentAgnle, times stepsPerDegree, rounded, will yield our instruction.
  // Angle precision will be preserved in code but non-stacking error will be present at the stepper.
  int steps = round((newAngle - currentAngle) * stepsPerDegree);

  // make the stepper move by the integer steps variable just defined above
  myStepper.step(steps);

  // update currentAngle variable to be equal to the newAngle
  currentAngle = newAngle; // Sure ok.
}
