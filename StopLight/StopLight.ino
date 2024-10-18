
int ButtonPin = 2;

int toggle = 0; 
int prevState = 0;
int curState = 0;

int LEDRed = 12;
int LEDYellow = 11;
int LEDGreen = 10;

void setup() {
  pinMode(LEDRed, OUTPUT);
  pinMode(LEDYellow, OUTPUT);
  pinMode(LEDGreen, OUTPUT);
  pinMode(ButtonPin, INPUT_PULLUP); // Set button to a sensor (+20kohms) pushed:low,not pushed:high

  Serial.begin(9600);

  // Start with the red LED on.
  digitalWrite(LEDRed, HIGH);

}

void loop() {

  curState = digitalRead(ButtonPin);

  if (curState < prevState) {
    Serial.println("New button press");

    // Switch active LED
    switch(toggle) {
      case 0:
        digitalWrite(LEDRed, LOW);
        digitalWrite(LEDGreen, HIGH);
        Serial.println("To green");
        toggle = 1;
        break;
      case 1:
        toggle = 2;
        digitalWrite(LEDGreen, LOW);
        digitalWrite(LEDYellow, HIGH);
        Serial.println("To yellow");
        break;
      case 2:
        digitalWrite(LEDYellow, LOW);
        digitalWrite(LEDRed, HIGH);
        Serial.println("To red");
        toggle = 0;
        break;
    }

    delay(200);
  }

  prevState = curState; 
}
