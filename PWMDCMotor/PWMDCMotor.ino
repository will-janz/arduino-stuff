// Pins
#define MOTOR 3

void setup() {
  pinMode(MOTOR, OUTPUT);

  Serial.begin(9600);
  
}

void loop() {
  // Spin fan up
  for (int i = 0; i <= 255; i += 1) {
    analogWrite(MOTOR, i);
    delay(20);
  }
  delay(1000);
  // Spin fan down
  for (int i = 255; i >= 0; i -= 1) {
    analogWrite(MOTOR, i);
    delay(20);
  }
}
