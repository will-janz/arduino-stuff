/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/

unsigned long previousMillis = 0; // Stores the last time LED was updated
const long interval = 1000; // Interval at which to blink (milliseconds)

int voltage = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  //  unsigned long currentMillis = millis(); // Get the current time
 
  // if (currentMillis - previousMillis >= interval) {
  //   previousMillis = currentMillis; // Save the last time LED was updated
  //   // Toggle the LED state
  //   if (digitalRead(LED_BUILTIN) == LOW) {
  //   digitalWrite(LED_BUILTIN, HIGH);
  //   } else {
  //   digitalWrite(LED_BUILTIN, LOW);
  //   }
  // }
 // Add other non-blocking tasks here


  // digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  // delay(100);                      // wait for a second
  // digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  // delay(100);                      // wait for a second
  // digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  // delay(100);                      // wait for a second
  // digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  // delay(1000);                      // wait for a second

  // if (digitalRead(7) == LOW) {
  //   digitalWrite(7, HIGH);
  //   digitalWrite(8, LOW);
  // } else {
  //   digitalWrite(7, LOW);
  //   digitalWrite(8, HIGH);
  // }


  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);

  voltage = analogRead(A0);
  Serial.println((float)voltage * 5 / 1023);

}
