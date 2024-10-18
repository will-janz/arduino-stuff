#include <IRremote.h>
#include <Servo.h>

enum REMOTE {
  POWER = 69,
  VOLUP = 70,
  FUNCSTOP = 71,
  REVERSE = 68,
  PLAYPAUSE = 64,
  FORWARD = 67,
  DOWN = 7,
  VOLDOWN = 21,
  UP = 9,
  NUM0 = 22,
  EQ = 25,
  STREPT = 13,
  NUM1 = 12,
  NUM2 = 24,
  NUM3 = 94,
  NUM4 = 8,
  NUM5 = 28,
  NUM6 = 90,
  NUM7 = 66,
  NUM8 = 82,
  NUM9 = 74
};

// Set up IR reciever pin
const byte IR_RECEIVE_PIN = 3;

// Create variable to store value recieved from the ir remote
enum REMOTE ir_command;
// Create Servo object
Servo myservo;
// Create servo angle variable, start at 90 degrees, can be any angle between 0 to 180 degrees
int servoangle = 90;
// Set an initial angle increment that the up/down arrows will move the servo by (in degrees)
int increment=10;

void setup()
{
   Serial.begin(9600);  // start serial monitor to visualize data
   IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // start IR receiver library
   Serial.println("IR Receiver Online");   // print initial message
   myservo.attach(9); // Servo signal wire on pin 9
   myservo.write(servoangle); // write the desired servo angle (servoangle) to the servo motor
   delay(20);  // every servoname.write(angle); function call needs a 20 ms delay or timer to update servo position
}

void loop()
{
      // Move servo to new position, just need to update servoangle variable inside IrReceiver().decode if statement below
      myservo.write(servoangle);  // write the desired servo angle (servoangle) to the servo motor
      delay(20);  // every servoname.write(angle); function call needs a 20 ms delay or timer to update servo position
      
   if (IrReceiver.decode()) // Only perform an action if an IR signal is read by the reciever from the remote
   {
      ir_command = IrReceiver.decodedIRData.command;   // Decode IR remote button press signal data
      Serial.println(ir_command); // Print decoded IR remote button press signal signal received

      /*   No changes needed above */

      
      /*  Use the serial monitor to figure out what code each remote button 
       *  gives when pressed.  Use in an if-else if case structure below to move the servo
       *  depending on which is button pressed. Make sure to press the buttons and set
       *  the case numbers to the appropriate button press. Two example case structures
       *  are shown below for reference, but need to be customized for your remote. 
       *  Remember to include a break statement after each case, and use Serial.println();
       *  statements for debugging to make sure the buttons are registering correctly.
       */
      
      switch (ir_command) {
        // If red power button is pushed, go to 90 degrees, include Serial.println(); statement
        case POWER:
          servoangle = 90;
          break;
        // Else if rewind forward (|<<) is pushed, go to 0 degrees, include Serial.println(); statement
        case REVERSE:
          servoangle = 0;
          break;
        // Else if fast forward (>>|) is pushed, go to 180 degrees, include Serial.println(); statement
        case FORWARD:
          servoangle = 180;
          break;
        // Else if UP arrow is pushed, increase servoangle variable by positive 'increment', include Serial.println(); statement
        case UP:
          servoangle += increment;
          break;
        // Else if DOWN arrow is pushed, decrease servoangle variable by negative 'increment', include Serial.println(); statement
        case DOWN:
          servoangle -= increment;
          break;
        // Else if VOL+ is pressed, adjust 'increment' up by 1, include a Serial.println(); print the increment
        case VOLUP:
          increment++;
          Serial.println("New increment: " + (String)increment);
          break;
        // Else if VOL- is pressed, adjust 'increment' down by 1, include a Serial.println(); print the  increment
        case VOLDOWN:
          increment--;
          Serial.println("New increment: " + (String)increment);
          break;
      }
      
      // Use an if statement here to make sure servoangle isn't greater than 180 (highest possible servo angle)
      servoangle = min(180, servoangle);
      // Use an if statement here to make sure servoangle isn't less than 0 (lowest possible servo angle)
      servoangle = max(0, servoangle);
      // Use an if statement here to make sure increment isn't less than 1 (lowest increment)
      increment = max(1, increment);


      /*   No changes needed below */

      // Prints the final values of 'increment' and servoangle - check these to make sure they don't exceed limits
      // and are responding correctly to button presses 
      Serial.print("***Increment = ");
      Serial.println(increment);
      Serial.print("***Servo Angle = ");
      Serial.println(servoangle);

      // must always have this line at the end of "if (IrReceiver.decode())"
      IrReceiver.resume();  // Resume waiting for the next IR remote button press
   }  // End IR receiver decode if statement after button pressed
} // End void loop, head back to the top of the void loop again

