
int LEDPin = 13;
int ButtonPin = 2;
bool toggle = false;
int prevState = 0;
int curState = 0;

int count = 0; // How many times the button has been pressed.

void setup() {
  pinMode(LEDPin, OUTPUT);
  pinMode(ButtonPin, INPUT_PULLUP); // Set button to a sensor (+20kohms) pushed:low,not pushed:high

  Serial.begin(9600);
}

void loop() {

  // Moment example
  // On button press
  // if (digitalRead(ButtonPin) == LOW) {
  //   count++;
  //   digitalWrite(LEDPin, HIGH);
  // delay(100);
  // } 
  // // On button not press
  // else {
  //   digitalWrite(LEDPin, LOW);
  // }

  // Serial.println(count);


  // Toggle example
  // if (digitalRead(ButtonPin) == LOW) {
  //   toggle = !toggle;

  //   count++;
  //   delay(200);
  // }

  // if(toggle) {
  //   digitalWrite(LEDPin, HIGH);
  // } else {
  //   digitalWrite(LEDPin, LOW);
  // }

  // Serial.println(count);

  // Better toggle example that accounts for holding the button.
  curState = digitalRead(ButtonPin);

  if (curState < prevState) {
    toggle = !toggle;
    count++;
    delay(200);
  }

  if (toggle) {
    digitalWrite(LEDPin, HIGH);
  } else {
    digitalWrite(LEDPin, LOW);
  }
  
  Serial.print(curState);
  Serial.print(" ");
  Serial.println(count);
  prevState = curState; 
}
