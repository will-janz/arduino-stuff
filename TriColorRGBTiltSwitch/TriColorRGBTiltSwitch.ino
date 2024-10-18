// Pins
#define RED 9
#define GREEN 10
#define BLUE 11
#define BUTTON 2

// Button state vars
// Or a tilt switch, wahtever.
int prevState = 0;
int curState = 0;
// LED state
int LEDState = 0; 

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {

  curState = digitalRead(BUTTON);

  if (curState < prevState) {
    Serial.println("New button press");

    switch (LEDState) {
      case 0:
        setLED(127, 0, 255); // Violet
        LEDState = 1;
        break;
      case 1:
        setLED(0, 255, 127); // Aquamarine
        LEDState = 2;
        break;
      case 2:
        setLED(255, 127, 0); // Orange
        LEDState = 0;
        break;
    }

    // Switch actuation debounce
    delay(500);
  }

  prevState = curState;
}

// Helper functions
void setLED(int r, int g, int b) {
  g = g * 0.35;
  b = b * 0.3;

  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
}