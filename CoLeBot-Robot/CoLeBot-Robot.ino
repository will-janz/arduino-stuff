#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Servo.h>
#include <printf.h>

RF24 radio(7, 8);  // CE, CSN

struct DataPacket {
  char canary;
  bool lButton;
  int  lDir;
  int  lServo;
  bool rButton;
  int  rDir;
  int  rServo;
};

DataPacket data;

const byte address[6] = "58008";

// Motor control pins - mapped to what worked on the robot
#define PIN_L_ENABLE 6
#define PIN_L_DIR_1 A3
#define PIN_L_DIR_2 A2
#define PIN_R_ENABLE 5
#define PIN_R_DIR_1 A1
#define PIN_R_DIR_2 A0

#define PIN_SERVO_1 2
#define PIN_SERVO_2 3

Servo servo1;
Servo servo2;

byte motorSpeed = 255;

float lServoAngle = 90.0;
float rServoAngle = 90.0;

long danceLast = 0;

void setup() {
  // Radio stuff
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0, address);
  radio.startListening();

  // Servo motors
  servo1.attach(PIN_SERVO_1);
  servo2.attach(PIN_SERVO_2);

  // DC motors
  pinMode(PIN_L_ENABLE, OUTPUT);
  pinMode(PIN_L_DIR_1, OUTPUT);
  pinMode(PIN_L_DIR_2, OUTPUT);
  pinMode(PIN_R_ENABLE, OUTPUT);
  pinMode(PIN_R_DIR_1, OUTPUT);
  pinMode(PIN_R_DIR_2, OUTPUT);

  Serial.begin(115200); // I want more throughput for this one
  // Serial.begin(9600);

  Serial.println("Robot initializing...");
  printf_begin(); // For radio debugging
}

void loop() {
  // Handling general radio failure
  // Restart if something, I don't know what, goes wrong.
  if(radio.failureDetected) {
    Serial.println("Failure detected!");
    delay(500);
    radio.begin();                          // Attempt to re-configure the radio with defaults
    radio.failureDetected = 0;              // Reset the detection value

    radio.setPALevel(RF24_PA_LOW);
    radio.openReadingPipe(0, address);
    radio.startListening();
  }

  // If the radio has some data for us.
  if (radio.available()) {
    radio.read(&data, sizeof(DataPacket));
    delay(50);

    // Sometimes the library thinks the radio has data for us but doesn't,
    // and it manifests as a dataset of all 0s. This often happens while it's disconnected.
    // Double check that we're actually connected, and exit the loop if we're not.
    if (radio.isChipConnected() == false) {
      Serial.println("Radio disconnected!");
      return;
    }

    // Sometimes everything looks like it's working, the radio thinks it's getting data,
    // the library thinks we have data with the right address, but for _some reason_,
    // we get all 0s when we aren't supposed to. The canary variable checks to make sure we
    // got actual data.
    if (data.canary != 'E') {
      Serial.println("Canary failed!");
      // radio.printPrettyDetails();
      // delay(6000);
      return;
    }

    // pragma regions are used for code folding.
    #pragma region Motors
    // Left motor
    if (data.lDir > 550) {
      data.lDir = map(data.lDir, 550, 1023, 0, motorSpeed);
      analogWrite(PIN_L_ENABLE, data.lDir);
      digitalWrite(PIN_L_DIR_1, HIGH);
      digitalWrite(PIN_L_DIR_2, LOW);

    } else if (data.lDir < 450){
      data.lDir = map(data.lDir, 450, 0, 0, motorSpeed);
      analogWrite(PIN_L_ENABLE, data.lDir);
      digitalWrite(PIN_L_DIR_1, LOW);
      digitalWrite(PIN_L_DIR_2, HIGH);

    } else {
      analogWrite(PIN_L_ENABLE, 0);
      digitalWrite(PIN_L_DIR_1, LOW);
      digitalWrite(PIN_L_DIR_2, LOW);
    }

    // Right motor - DIRECTION OPPOSITE LEFT
    if (data.rDir > 550) {
      data.rDir = map(data.rDir, 550, 1023, 0, motorSpeed);
      analogWrite(PIN_R_ENABLE, data.rDir);
      digitalWrite(PIN_R_DIR_1, LOW); // INVERT
      digitalWrite(PIN_R_DIR_2, HIGH); // INVERT

    } else if (data.rDir < 450){
      data.rDir = map(data.rDir, 450, 0, 0, motorSpeed);
      analogWrite(PIN_R_ENABLE, data.rDir);
      digitalWrite(PIN_R_DIR_1, HIGH); // INVERT
      digitalWrite(PIN_R_DIR_2, LOW); // INVERT

    } else {
      analogWrite(PIN_R_ENABLE, 0);
      digitalWrite(PIN_R_DIR_1, LOW);
      digitalWrite(PIN_R_DIR_2, LOW);
    }
    #pragma endregion Motors

    #pragma region Servos
    // Servo 1, or servo L
    // Servo L maps to servo 1, which should be the main arm.
    if (data.lServo > 600) {
      lServoAngle += 3;
    } else if (data.lServo < 400) {
      lServoAngle -= 3;
    }

    // Servo R maps to servo 2, which should be the claw
    if (data.rServo > 600) {
      rServoAngle += 3;
    } else if (data.rServo < 400) {
      rServoAngle -= 3;
    }

    // Clamp servo positions
    lServoAngle = constrain(lServoAngle, 45, 150); // Arm limits
    servo1.write(lServoAngle);

    rServoAngle = constrain(rServoAngle, 50, 126); // Claw limits
    servo2.write(rServoAngle);
    #pragma endregion Servos

    // Bunch of debug information
    Serial.println("lButt: " + (String)data.lButton + ", lServo: " + (String)data.lServo + ", lDir: " + (String)data.lDir + ", rButt: " + (String)data.rButton + ", rServo: " + (String)data.rServo + ", rDir: " + (String)data.rDir);
    Serial.println("lServo: " + (String)lServoAngle + ", rServo: " + (String)rServoAngle + ", speed: " + (String)motorSpeed);
  }

  // Right button does a hastily thrown together dance routine.
  if (data.rButton && millis() - danceLast >= 2000 ) {
    dance();
    danceLast = millis();
  }

  // Left button changes DC motor speed.
  // Uses the danceLast for debouncing because I don't see the need
  // for another variable.
  if (data.lButton && millis() - danceLast >= 2000) {
    motorSpeed = motorSpeed == 255 ? 150 : 255;
    danceLast = millis();
  }
}

// This makes the robot twerk, kinda.
void dance() {
  // Left forward
  // analogWrite(PIN_L_ENABLE, data.lDir);
  // digitalWrite(PIN_L_DIR_1, HIGH);
  // digitalWrite(PIN_L_DIR_2, LOW);

  // Right forward
  // analogWrite(PIN_R_ENABLE, data.rDir);
  // digitalWrite(PIN_R_DIR_1, LOW); // INVERT
  // digitalWrite(PIN_R_DIR_2, HIGH); // INVERT


  // Left forward
  analogWrite(PIN_L_ENABLE, 255);
  digitalWrite(PIN_L_DIR_1, HIGH);
  digitalWrite(PIN_L_DIR_2, LOW);

  // Right backward
  analogWrite(PIN_R_ENABLE, 255);
  digitalWrite(PIN_R_DIR_1, HIGH); // INVERT
  digitalWrite(PIN_R_DIR_2, LOW); // INVERT

  delay(100);

  // Left backward
  analogWrite(PIN_L_ENABLE, 255);
  digitalWrite(PIN_L_DIR_1, LOW);
  digitalWrite(PIN_L_DIR_2, HIGH);

  // Right forward
  analogWrite(PIN_R_ENABLE, 255);
  digitalWrite(PIN_R_DIR_1, LOW); // INVERT
  digitalWrite(PIN_R_DIR_2, HIGH); // INVERT

  delay(100);


  // Left forward
  analogWrite(PIN_L_ENABLE, 255);
  digitalWrite(PIN_L_DIR_1, HIGH);
  digitalWrite(PIN_L_DIR_2, LOW);

  // Right backward
  analogWrite(PIN_R_ENABLE, 255);
  digitalWrite(PIN_R_DIR_1, HIGH); // INVERT
  digitalWrite(PIN_R_DIR_2, LOW); // INVERT

  delay(100);

  // Left backward
  analogWrite(PIN_L_ENABLE, 255);
  digitalWrite(PIN_L_DIR_1, LOW);
  digitalWrite(PIN_L_DIR_2, HIGH);

  // Right forward
  analogWrite(PIN_R_ENABLE, 255);
  digitalWrite(PIN_R_DIR_1, LOW); // INVERT
  digitalWrite(PIN_R_DIR_2, HIGH); // INVERT

  delay(100);

  // Left forward
  analogWrite(PIN_L_ENABLE, 255);
  digitalWrite(PIN_L_DIR_1, HIGH);
  digitalWrite(PIN_L_DIR_2, LOW);

  // Right backward
  analogWrite(PIN_R_ENABLE, 255);
  digitalWrite(PIN_R_DIR_1, HIGH); // INVERT
  digitalWrite(PIN_R_DIR_2, LOW); // INVERT

  delay(100);

  // Left backward
  analogWrite(PIN_L_ENABLE, 255);
  digitalWrite(PIN_L_DIR_1, LOW);
  digitalWrite(PIN_L_DIR_2, HIGH);

  // Right forward
  analogWrite(PIN_R_ENABLE, 255);
  digitalWrite(PIN_R_DIR_1, LOW); // INVERT
  digitalWrite(PIN_R_DIR_2, HIGH); // INVERT

  delay(100);

  // Left forward
  analogWrite(PIN_L_ENABLE, 255);
  digitalWrite(PIN_L_DIR_1, HIGH);
  digitalWrite(PIN_L_DIR_2, LOW);

  // Right backward
  analogWrite(PIN_R_ENABLE, 255);
  digitalWrite(PIN_R_DIR_1, HIGH); // INVERT
  digitalWrite(PIN_R_DIR_2, LOW); // INVERT

  delay(100);

  // Left backward
  analogWrite(PIN_L_ENABLE, 255);
  digitalWrite(PIN_L_DIR_1, LOW);
  digitalWrite(PIN_L_DIR_2, HIGH);

  // Right forward
  analogWrite(PIN_R_ENABLE, 255);
  digitalWrite(PIN_R_DIR_1, LOW); // INVERT
  digitalWrite(PIN_R_DIR_2, HIGH); // INVERT

  delay(100);

  // Left forward
  analogWrite(PIN_L_ENABLE, 255);
  digitalWrite(PIN_L_DIR_1, HIGH);
  digitalWrite(PIN_L_DIR_2, LOW);

  // Right backward
  analogWrite(PIN_R_ENABLE, 255);
  digitalWrite(PIN_R_DIR_1, HIGH); // INVERT
  digitalWrite(PIN_R_DIR_2, LOW); // INVERT

  delay(100);

  // Left backward
  analogWrite(PIN_L_ENABLE, 255);
  digitalWrite(PIN_L_DIR_1, LOW);
  digitalWrite(PIN_L_DIR_2, HIGH);

  // Right forward
  analogWrite(PIN_R_ENABLE, 255);
  digitalWrite(PIN_R_DIR_1, LOW); // INVERT
  digitalWrite(PIN_R_DIR_2, HIGH); // INVERT

  // All off

  analogWrite(PIN_L_ENABLE, 0);
  digitalWrite(PIN_L_DIR_1, LOW);
  digitalWrite(PIN_L_DIR_2, LOW);
  analogWrite(PIN_R_ENABLE, 0);
  digitalWrite(PIN_R_DIR_1, LOW); // INVERT
  digitalWrite(PIN_R_DIR_2, LOW); // INVERT


}
