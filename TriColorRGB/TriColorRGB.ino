// Pins
#define RED 9
#define GREEN 10
#define BLUE 11

// Vars
int redValue;
int greenValue;
int blueValue;

void setup(){
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

// main loop
void loop(){
  setLED(127, 0, 255); // Violet
  delay(1000);
  setLED(0, 255, 127); // Aquamarine
  delay(1000);
  setLED(255, 127, 0); // Orange
  delay(1000);
}

// Helper functions
void setLED(int r, int g, int b) {
  g = g * 0.35;
  b = b * 0.3;

  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
}