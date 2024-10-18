// Pins
#define RED 9
#define GREEN 10
#define BLUE 11

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  Serial.begin(9600);
  
  setLED(255, 0, 255); // Magenta
}

void loop() {
  // Fade smoothly in a cycle magenta, cyan, yellow, magenta, etc.

  // To cyan
  // red to 0, green to 255
  for (int i = 0; i <= 255; i += 1) {
    setLED(255 - i, i, 255);
    delay(20);
  }

  // To yellow
  // Red to 255, blue to 0
  for (int i = 0; i <= 255; i += 1) {
    setLED(i, 255, 255 - i);
    delay(20);
  }

  // To magenta
  // Green to 0, blue to 255
  for (int i = 0; i <= 255; i += 1) {
    setLED(255, 255 - i, i);
    delay(20);
  }
}

// Helper functions
void setLED(int r, int g, int b) {
  g = g * 0.35;
  b = b * 0.3;

  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
}