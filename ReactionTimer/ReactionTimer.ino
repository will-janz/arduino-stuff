// MEGN 200 Team

/*
  LCD stopwatch example code
  The circuit:
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 8
 * LCD D4 pin to digital pin 9
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 11
 * LCD D7 pin to digital pin 12
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

#include <LiquidCrystal.h> // include the library code
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // initialize the library with the numbers of the interface pins

int pushButton = 2; // pushbutton pin
int restartButton = 3;
int led = 5;
int times[5];
int tryNum = 0;

unsigned long startTime = 0;  // initial startTime timing variable starting at 0
unsigned long duration = 0;  // initial duration (elapsed time) timing variable starting at 0
// TODO: Include any global variables that you need for your button!
bool started = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); // set up the LCD's number of columns and rows:
  lcd.print("Hello Stopwatch!");    // print out this initial text to the LCD screen
  pinMode(pushButton, INPUT_PULLUP);   // setup the pushbutton with the INPUT PULLUP resistor
  pinMode(restartButton, INPUT_PULLUP);
  pinMode(led, OUTPUT);   // setup the pushbutton with the INPUT PULLUP resistor
}

void loop() {
  if (digitalRead(restartButton) == LOW) {
    clearlcd();
    lcd.print("Restarting");
    digitalWrite(led, LOW);
    started = 0;
    tryNum = 0; // Start the trials over
    delay(1000);
  }

  if (digitalRead(pushButton) == LOW && started == 1) {  // if the button is pressed
    /* 
    **TODO**
    1. Calculate how long it has been since startTime.
    2. Turn off your LED.
    3. Print the reaction time to your LCD.
    4. Add a 3 second delay (or so) so that the user can read their results.
    5. Reset your state variable so that the timer will start again.
    // no changes should be needed below
    */
    duration = millis() - startTime;
    digitalWrite(led, LOW);
    clearlcd();
    lcd.setCursor(0,0);
    lcd.print("Reaction time:");
    lcd.setCursor(0,1);
    lcd.print(duration);

    // Record reaction time
    times[tryNum] = duration;

    // If we just did our 5th test
    if (tryNum == 4) {
      int summation = 0;
      // Here's the for loop you wanted
      for (byte i = 0; i < 5; i = i + 1) {
        Serial.println("Trial " + (String)i + ": " + (String)times[i]);
        summation += times[i];
      }

      // Print out average
      lcd.setCursor(0,0);
      lcd.print("Average time:");
      lcd.setCursor(0,1);
      lcd.print(summation / 5);

      // Wait 10 seconds
      delay(10000);

      // Start over
      tryNum = 0;
    }
    else {
      // Set for next try
      tryNum++;
      delay(3000);
    }

    started = 0;
  }
  else if (!started) {  // if the button is not pressed and not started
    started = true;  // Sets the state variable to true, to skip this section of code next time.
    
    int delayTime = random(500, 5000); // This chooses a random time between 0.5 and 5 seconds.
    
    clearlcd(); // A function included for your convenience (see below)
    lcd.print("Get Ready");
    delay(delayTime);
   
    clearlcd();
    lcd.print("Now!");
    digitalWrite(led, HIGH);
    startTime = millis();
    
  }
}

void clearlcd() {
  /*
   * This function clears the LCD display. Helpful before new text is written on it.
   */
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 0);
}
