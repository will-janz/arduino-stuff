int LEDPin = 13;
int ButtonPin = 2;
bool toggle = false;
int prevState = 0;
int curState = 0;

void setup() {
  pinMode(LEDPin, OUTPUT);
  pinMode(ButtonPin, INPUT_PULLUP); // Set button to a sensor (+20kohms) pushed:low,not pushed:high

  Serial.begin(9600);

}

void loop() {

  curState = digitalRead(ButtonPin);

  if (curState < prevState) {
    Serial.println("New button press");
    toggle = !toggle;
    delay(200);
  }

  if(toggle) {
    digitalWrite(LEDPin, HIGH);
    Serial.println("LED ON");
  } else {
    digitalWrite(LEDPin, LOW);
    Serial.println("LED OFF");
  }

  prevState = curState; 
}
