int prVal = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT); 
}

void loop() {
  prVal = analogRead(A0);

  if (prVal > 512) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }

  Serial.print("Raw: ");
  Serial.print(prVal);
  Serial.print(", Volts: ");
  Serial.println((float)prVal * 5 / 1023);
  delay(10);
}
