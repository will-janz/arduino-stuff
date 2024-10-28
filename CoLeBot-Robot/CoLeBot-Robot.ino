#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Servo.h>
#include <printf.h>

RF24 radio(7, 8);  // CE, CSN

struct DataPacket {
  char check;
  bool lButton;
  int  lDir;
  int  lServo;
  bool rButton;
  int  rDir;
  int  rServo;
  char sum;
};

DataPacket data; 

const byte address[6] = "58008";
// const byte address2[6] = "58009";

// Is it DIR 1 and 2 or DIR R and F?
#define PIN_L_ENABLE 5
#define PIN_L_DIR_1 A0
#define PIN_L_DIR_2 A1
#define PIN_R_ENABLE 6
#define PIN_R_DIR_1 A2
#define PIN_R_DIR_2 A3

#define PIN_SERVO_1 2
#define PIN_SERVO_2 3

Servo servo1;
Servo servo2;

float lServoAngle = 90.0;
float rServoAngle = 90.0;

void setup() {
  // Radio stuff
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0, address);
  radio.startListening();

  // servo motors
  servo1.attach(PIN_SERVO_1);
  servo2.attach(PIN_SERVO_2);

  // DC motors
  pinMode(PIN_L_ENABLE, OUTPUT);
  pinMode(PIN_L_DIR_1, OUTPUT);
  pinMode(PIN_L_DIR_2, OUTPUT);
  pinMode(PIN_R_ENABLE, OUTPUT);
  pinMode(PIN_R_DIR_1, OUTPUT);
  pinMode(PIN_R_DIR_2, OUTPUT);

  // Serial.begin(115200);
  Serial.begin(9600);
  Serial.println("Robot initializing...");
  printf_begin();
}

void loop() {
  if(radio.failureDetected) {
    Serial.println("Failure detected!");
    delay(500);
    radio.begin();                          // Attempt to re-configure the radio with defaults
    radio.failureDetected = 0;              // Reset the detection value

    radio.setPALevel(RF24_PA_LOW);
    radio.openReadingPipe(0, address);
    radio.startListening();
  }

  //delay(20);//20ms delay to get signal
  if (radio.available()) {
    radio.read(&data, sizeof(DataPacket));
    delay(50);
    
    if (radio.isChipConnected() == false) {
      Serial.println("Radio disconnected!");
      return;
    }
    
    if (data.check != 'E' || data.sum != 'E') {
      Serial.println("Checksum failed!");
      return;
    }

    // if (data.lServo == 0) {
    //   radio.printPrettyDetails();
    //   delay(6000);
    // }

    // radio.printPrettyDetails();

    #pragma region Motors
    // Left motor
    if (data.lDir > 600) {
      data.lDir = map(data.lDir, 600, 1023, 0, 255);
      analogWrite(PIN_L_ENABLE, data.lDir);
      // use two digitalWrites on the dir pins to make the motor spin in one direction (clockwise)
      digitalWrite(PIN_L_DIR_1, HIGH);
      digitalWrite(PIN_L_DIR_2, LOW);
      
    } else if (data.lDir < 400){
      data.lDir = map(data.lDir, 400, 0, 0, 255);
      analogWrite(PIN_L_ENABLE, data.lDir);
      // use two digitalWrites on the dir pins to make the motor spin in the other direction (counter-clockwise)
      digitalWrite(PIN_L_DIR_1, LOW);
      digitalWrite(PIN_L_DIR_2, HIGH);
      
    } else {
      analogWrite(PIN_L_ENABLE, 0);
      // use two digitalWrites on the dir pins to make the motor not spin in either direction (i.e. set both LOW)
      digitalWrite(PIN_L_DIR_1, LOW);
      digitalWrite(PIN_L_DIR_2, LOW);
    }

    // Right motor - DIRECTION OPPOSITE LEFT
    if (data.rDir > 600) {
      data.rDir = map(data.rDir, 600, 1023, 0, 255);
      analogWrite(PIN_R_ENABLE, data.rDir);
      // use two digitalWrites on the dir pins to make the motor spin in one direction (clockwise)
      digitalWrite(PIN_R_DIR_1, LOW); // INVERT
      digitalWrite(PIN_R_DIR_2, HIGH); // INVERT
      
    } else if (data.rDir < 400){
      data.rDir = map(data.rDir, 400, 0, 0, 255);
      analogWrite(PIN_R_ENABLE, data.rDir);
      // use two digitalWrites on the dir pins to make the motor spin in the other direction (counter-clockwise)
      digitalWrite(PIN_R_DIR_1, HIGH); // INVERT
      digitalWrite(PIN_R_DIR_2, LOW); // INVERT
      
    } else {
      analogWrite(PIN_R_ENABLE, 0);
      // use two digitalWrites on the dir pins to make the motor not spin in either direction (i.e. set both LOW)
      digitalWrite(PIN_R_DIR_1, LOW);
      digitalWrite(PIN_R_DIR_2, LOW);
    }
    #pragma endregion Motors

    #pragma region Servos
    // Servo 1, or servo L
    // Mirror control
    // data.lServo = map(data.lServo, 0, 1023, 0, 180);
    // servo1.write(data.lServo);\usepackage{steinmetz}
    // Adjust control 
    if (data.lServo > 600) {
      lServoAngle += 3;
    } else if (data.lServo < 400) {
      lServoAngle -= 3;
    }

    if (data.rServo > 600) {
      rServoAngle += 3;
    } else if (data.rServo < 400) {
      rServoAngle -= 3;
    }
    // Clamp servo positions
    lServoAngle = constrain(lServoAngle, 20, 110);
    servo1.write(lServoAngle);

    rServoAngle = constrain(rServoAngle, 20, 110);
    servo2.write(rServoAngle);
    #pragma endregion Servos

    Serial.println("lButt: " + (String)data.lButton + ", lServo: " + (String)data.lServo + ", lDir: " + (String)data.lDir + ", rButt: " + (String)data.rButton + ", rServo: " + (String)data.rServo + ", rDir: " + (String)data.rDir);
  } 
  // else {
  //   radio.printPrettyDetails();  
  //   delay(1000);
  // }
}