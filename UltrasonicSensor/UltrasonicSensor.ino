#include "SR04.h"
#define TRIG_PIN 12
#define ECHO_PIN 11
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);

long a;

#define R_PIN 6
#define G_PIN 5
#define B_PIN 3
#define BUZZER 8

void setup() {
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);

  delay(1000);
}

void loop() {
  a = sr04.Distance();

  Serial.print(a);
  Serial.println("cm");


  if (a < 7) {
    // Red LED
    setLED(255, 0, 0);
    // Buzzer
    digitalWrite(BUZZER, HIGH);
  } else if (a > 8 && a < 13) {
    // Yellow LED
    setLED(255, 255, 0);
    // Chirp the buzzer

    digitalWrite(BUZZER, HIGH);
    delay(50);
    digitalWrite(BUZZER, LOW);
    delay(200);
  } else if (a > 14) {
    // Green LED
    setLED(0, 255, 0);
    digitalWrite(BUZZER, LOW);
  }

   delay(1000);
}

// Helper functions
void setLED(int r, int g, int b) {
  g = g * 0.35;
  b = b * 0.3;

  analogWrite(R_PIN, r);
  analogWrite(G_PIN, g);
  analogWrite(B_PIN, b);
}