#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Servo.h>

RF24 radio(9, 8);  // CE, CSN

struct DataPacket {
  bool lButton;
  int  lDir;
  int  lServo;
  bool rButton;
  int  rDir;
  int  rServo;
};

struct backDataPacket {
  int heartbeat;
};

DataPacket data; 
backDataPacket backData;

const byte address[6] = "58008";
const byte address2[6] = "58009";

// Is it DIR 1 and 2 or DIR R and F?
#define PIN_L_ENABLE 6
#define PIN_L_DIR_1 7
#define PIN_L_DIR_2 5
#define PIN_R_ENABLE 1
#define PIN_R_DIR_1 1
#define PIN_R_DIR_2 1

#define PIN_SERVO_1 3
#define PIN_SERVO_2 1

Servo servo1;
Servo servo2;

void setup() {
  // Radio stuff
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0, address);
  radio.startListening();
  // radio.openWritingPipe(address2);

  // DC motors
  // TODO

  // servo motors
  servo1.attach(PIN_SERVO_1);
  servo2.attach(PIN_SERVO_2);


  Serial.begin(9600);
}

void loop() {
  //delay(20);//20ms delay to get signal
  if (radio.available()) {
    radio.read(&data, sizeof(DataPacket));
    delay(20);

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
    #pragma endregion Motors

    #pragma Servos
    data.lServo = map(data.lServo, 0, 1023, 0, 180);
    servo1.write(data.lServo);
    // servo1.write(0);
    #pragma endregion Servos

    Serial.println("lButt: " + (String)data.lButton + ", lServo: " + (String)data.lServo + ", lDir: " + (String)data.lDir);
  }
}