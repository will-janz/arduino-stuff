#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
// OLED/LCD libraries for extra fun
#include "ssd1306.h"
#include "nano_gfx.h"

// Define our radio
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

// Joystick buttons and pots
#define PIN_JOY_L_BUTTON 4
#define PIN_JOY_L_X A0
#define PIN_JOY_L_Y A1
#define PIN_JOY_R_BUTTON 5
#define PIN_JOY_R_X A2
#define PIN_JOY_R_Y A3

void setup() {
  // pragma regions are used for code folding.
  #pragma region OLED init
  // Select the font to use with menu and all font functions
  ssd1306_setFixedFont(ssd1306xled_font8x16);
  ssd1306_128x32_i2c_init();

  ssd1306_clearScreen();
  ssd1306_printFixed(0,  0, "Initializing... ", STYLE_NORMAL);
  #pragma endregion OLED init

  // Allocate our pins.
  pinMode(PIN_JOY_L_BUTTON, INPUT_PULLUP);
  pinMode(PIN_JOY_L_X, INPUT);
  pinMode(PIN_JOY_L_Y, INPUT);
  pinMode(PIN_JOY_R_BUTTON, INPUT_PULLUP);
  pinMode(PIN_JOY_R_X, INPUT);
  pinMode(PIN_JOY_R_Y, INPUT);

  // Start up our radio.
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);
  radio.stopListening();

  // Start serial.
  Serial.begin(9600);
  Serial.println("Controller initializing...");

  // For data validation.
  data.canary = 'E';
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

  // Note if radio is disconnected, and don't bother trying to send data.
  if (radio.isChipConnected() == false) {
    Serial.println("Radio disconnected!");
    delay(50);
    return;
  }

  // Joystick buttons.
  // Not debouncing here since the whole loop is on a delay for data transmission.
  if (digitalRead(PIN_JOY_L_BUTTON) == LOW) {
    data.lButton = true;
  } else {
    data.lButton = false;
  }

  if (digitalRead(PIN_JOY_R_BUTTON) == LOW) {
    data.rButton = true;
  } else {
    data.rButton = false;
  }

  // Read pots.
  data.lServo = analogRead(PIN_JOY_L_Y);
  data.lDir = analogRead(PIN_JOY_L_X);
  data.rServo = analogRead(PIN_JOY_R_Y);
  data.rDir = analogRead(PIN_JOY_R_X);

  // Debugging.
  Serial.println("lButt: " + (String)data.lButton + ", lServo: " + (String)data.lServo + ", lDir: " + (String)data.lDir + ", rButt: " + (String)data.rButton + ", rServo: " + (String)data.rServo + ", rDir: " + (String)data.rDir);

  // Transmit data.
  radio.write(&data, sizeof(DataPacket));

  // Wait a bit.
  delay(50);
}

/**
 * Set text on the mini display.
 *
 * @param message Manually space padded message to put on the display. Should total 16 characters.
 */
void setLCD(String message) {
  ssd1306_printFixed(0,  0, message, STYLE_NORMAL);
}
