int prVal = 0;

void setup() {
  pinMode(12, OUTPUT); // Buzzer
  pinMode(13, OUTPUT); // LED
  pinMode(A0, INPUT); // Phresistor sense

  Serial.begin(9600);
}

void loop() {
  prVal = analogRead(A0);

  // Blink and buzz for a bit
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  delay(200);

  // Then don't
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);

  // Delay for a time based on the photoresistor's resisitance
  Serial.println(prVal);
  delay(prVal);

}
