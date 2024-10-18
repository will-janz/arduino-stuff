#include <Stepper.h>

const int stepsPerRevolution = 2048; // number of steps to complete one turn of the stepper
int joystick=A0; // Read joystick on this pin
int joystickValue; // Variable to store analog read value
int button=2; // Pin to read button presses
int buttonState; // Variable to store button status
float newAngle=0; // Angle we command the stepper to go to
float currentAngle = 0; // The current position of the stepper, global variable set in function
int home=0; // store home angle location
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  myStepper.setSpeed(10); // set the speed at 10 rpm:
  Serial.begin(9600); // initialize the serial port:
  pinMode(button, INPUT_PULLUP); // Configure pin to read button
}

void loop() {
  // put your main code here, to run repeatedly:
  joystickValue=analogRead(joystick); // Read the joystick value
  buttonState=digitalRead(button); // Check if home button is pressed
  if (buttonState==LOW){ // If the button on the joystick is pressed, go to home location
    currentAngle=stepperGoTo(home,currentAngle);// Order stepper home
    newAngle=home;// Update angle variables
    delay(10); // Short delay to let stepper move
  }
  else{ // If the button is not pressed
    if (joystickValue>550 || joystickValue<450){ // Ignore small readings from joystick, reduces jittering
      newAngle+=map(joystickValue,0,1023,-5,5);// use the joystick to increment the newAngle variable
    }

  currentAngle=stepperGoTo(newAngle,currentAngle); // Move to the new angle
  Serial.print("newAngle= "); // Serial prints for debugging
  Serial.print(newAngle);
  Serial.print(" currentAngle= ");
  Serial.println(currentAngle);
  delay(10); // Short delay to let stepper move
  }
}

// ** No changes needed above, just update the function below ***
// Edit this function below to get the stepper motor to respond to the joystick inputs
// Custom function to move the stepper from the current angle to the new angle input
float stepperGoTo(float newAngle, float currentAngle) {
  // define "stepsPerDegree" as an integer equal to 6 steps/angular degree, which is the approximate # of integer steps per degree of motion
  // There are 5.68888 repeating (or 256/45) steps per degree. Using an integer here would quickly lead to positional innacuracies, so I've elected to bring in floats.
  // Every 256 steps is 45 degrees
  float stepsPerDegree = 2048.0 / 360.0; // This should go into the global space.

  // define integer steps variable as the difference between the newAngle and the current input variables multipled by the stepsPerDegree variable to convert the change in angle to number of steps
  // Difference of newAngle and CurrentAgnle, times stepsPerDegree, rounded, will yield our instruction.
  // Angle precision will be preserved in code but non-stacking error will be present at the stepper.
  int steps = round((newAngle - currentAngle) * stepsPerDegree);

  // make the stepper move by the integer steps variable just defined above
  myStepper.step(steps);

  // update currentAngle variable to be equal to the newAngle
  currentAngle = newAngle; // Sure ok.

  return currentAngle;  // this line will return the current angle value back into the void loop to keep track of the currentAngle after the stepper motion
}