//include the necessary libraries for the RF24 Tx/Rx
//Tx = transmitter
//Rx = reciever
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


// OLED
#include "ssd1306.h"
#include "nano_gfx.h"
// END OLED


RF24 radio(7, 8);  // CE, CSN
//this object is recognized by the RF24 library
//and takes the two pin arguments for the CE and CSN pin
//CE is basically the input pin and is used to control data transmission
//CSN pin is a chip selection pin and turns on and off to recieve or transmit signal
//if you followed the provided wiring diagram then you do not need to change these

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

//you can change the 5-digit number but do not change the variable initialization or name
//those are necessary for the RF24 library

#define PIN_JOY_L_BUTTON 4
#define PIN_JOY_L_X A0
#define PIN_JOY_L_Y A1
#define PIN_JOY_R_BUTTON 5
#define PIN_JOY_R_X A2
#define PIN_JOY_R_Y A3

void setup() {
  #pragma region OLED init
  /* Select the font to use with menu and all font functions */
  ssd1306_setFixedFont(ssd1306xled_font8x16);
  ssd1306_128x32_i2c_init();

  ssd1306_clearScreen();
  ssd1306_printFixed(0,  0, "Initializing... ", STYLE_NORMAL);
  // ssd1306_createMenu( &menu, menuItems, sizeof(menuItems) / sizeof(char *) );
  // ssd1306_showMenu( &menu );
  #pragma endregion OLED init

  pinMode(PIN_JOY_L_BUTTON, INPUT_PULLUP);
  pinMode(PIN_JOY_L_X, INPUT);
  pinMode(PIN_JOY_L_Y, INPUT);
  pinMode(PIN_JOY_R_BUTTON, INPUT_PULLUP);
  pinMode(PIN_JOY_R_X, INPUT);
  pinMode(PIN_JOY_R_Y, INPUT);

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);

  radio.stopListening();
  // radio.openReadingPipe(0, address2);
  // radio.startListening();

  Serial.begin(9600);
  Serial.println("Controller initializing...");

  data.check = 'E';
  data.sum = 'E';
}

void loop() {
  // Radio checks
  if (radio.failureDetected) {
    Serial.println("Failure detected!");
    delay(500);
    radio.begin();                          // Attempt to re-configure the radio with defaults
    radio.failureDetected = 0;              // Reset the detection value

    radio.setPALevel(RF24_PA_LOW);
    radio.openReadingPipe(0, address);
    radio.startListening();
  }

  if (radio.isChipConnected() == false) {
    Serial.println("Radio disconnected!");
    delay(50);
    return;
  }

  // On joystick button press
  if (digitalRead(PIN_JOY_L_BUTTON) == LOW) {
    data.lButton = true;
    // delay(200); // TODO replace with millis
  } else {
    data.lButton = false;
  }

  // On joystick button press
  if (digitalRead(PIN_JOY_R_BUTTON) == LOW) {
    data.rButton = true;
    // delay(200); // TODO replace with millis
  } else {
    data.rButton = false;
  }

  data.lServo = analogRead(PIN_JOY_L_Y); 
  data.lDir = analogRead(PIN_JOY_L_X);
  data.rServo = analogRead(PIN_JOY_R_Y); 
  data.rDir = analogRead(PIN_JOY_R_X);

  Serial.println("lButt: " + (String)data.lButton + ", lServo: " + (String)data.lServo + ", lDir: " + (String)data.lDir + ", rButt: " + (String)data.rButton + ", rServo: " + (String)data.rServo + ", rDir: " + (String)data.rDir);

  radio.write(&data, sizeof(DataPacket));//transmit data

  delay(50); // Transmission delay

}

void setLCD(String message) {
  ssd1306_printFixed (0,  0, "NO COMM", STYLE_NORMAL);
}

