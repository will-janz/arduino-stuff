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

int pushbutton1 = 2; // pushbutton 1 pin
int pushbutton2 = 3; // pushbutton 2 pin
unsigned long startTime = 0;  // initial startTime timing variable starting at 0
unsigned long duration = 0;  // initial duration (elapsed time) timing variable starting at 0
bool timing = 0; // boolean state variable to indicate whether stopwatch is actively timing or not

void setup() {
  lcd.begin(16, 2); // set up the LCD's number of columns and rows:
  lcd.print("Hello Stopwatch!");    // print out this initial text to the LCD screen
  pinMode(pushbutton1, INPUT_PULLUP);   // setup the pushbutton with the INPUT PULLUP resistor
  pinMode(pushbutton2, INPUT_PULLUP);   // setup the pushbutton with the INPUT PULLUP resistor
}

void loop() {
  // if (digitalRead(pushbutton1) is LOW and boolean timing state variable is 0)
  if (digitalRead(pushbutton1) == LOW && timing == 0) {
    // set timing variable to 1 to indicate that timing has started
    timing = 1;
    // reset the startTime to the current time when the button is pressed using the millis() function
    startTime = millis();
    // clear lcd screen using lcd.clear() function to reset screen text
    lcd.clear();
    // set the cursor to column 0, row 0 (note: row 0 is the first row, since counting begins with 0)
    lcd.setCursor(0, 0);
    // print "Timing" on the first LCD row
    lcd.print("Timing");
    // button debounce delay of about 50-100 ms
    delay(100);
  // end if statement
  }
  // if (digitalRead(pushbutton2) is LOW and boolean timing state variable is 1)
  if (digitalRead(pushbutton2) == LOW && timing == 1) {
    // set timing variable to 0 to indicate that timing has stopped
    timing = 0;
    // define duration variable as the time elapsed in ms between the current time (millis()) and startTime
    duration = millis() - startTime;
    // clear lcd screen
    lcd.clear();
    // set the cursor to column 0, row 0
    lcd.setCursor(0, 0);
    // print "Done" on the first LCD row
    lcd.print("Done");
    // set the cursor to column 0, row 1 (note: row  1 is the second row, since counting begins with 0)
    lcd.setCursor(0, 1);
    // print "Duration: " on the second LCD row
    lcd.print("Duration: ");
    // print the duration variable value (time elapsed between pressing buttons)
    lcd.print(duration);
    // button debounce delay of about 50-100 ms
    delay(100);
  // end if statement
  }
}
