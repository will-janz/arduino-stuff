//include the necessary libraries for the RF24 Tx/Rx
//Tx = transmitter
//Rx = reciever
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


// OLED
#include "ssd1306.h"
#include "nano_gfx.h"
// #include "sova.h"

/*
 * Heart image below is defined directly in flash memory.
 * This reduces SRAM consumption.
 * The image is defined from bottom to top (bits), from left to
 * right (bytes).
 */
const PROGMEM uint8_t heartImage[8] =
{
    0B00001110,
    0B00011111,
    0B00111111,
    0B01111110,
    0B01111110,
    0B00111101,
    0B00011001,
    0B00001110
};

/*
 * Define sprite width. The width can be of any size.
 * But sprite height is always assumed to be 8 pixels
 * (number of bits in single byte).
 */
const int spriteWidth = sizeof(heartImage);

SAppMenu menu;

const char *menuItems[] =
{
    "draw bitmap",
    "sprites",
    "fonts",
    "canvas gfx",
    "draw lines",
};
// END OLED


RF24 radio(9, 8);  // CE, CSN
//this object is recognized by the RF24 library
//and takes the two pin arguments for the CE and CSN pin
//CE is basically the input pin and is used to control data transmission
//CSN pin is a chip selection pin and turns on and off to recieve or transmit signal
//if you followed the provided wiring diagram then you do not need to change these

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

//you can change the 5-digit number but do not change the variable initialization or name
//those are necessary for the RF24 library

#define PIN_JOY_L_BUTTON 4
#define PIN_JOY_L_X A0
#define PIN_JOY_L_Y A1
#define PIN_JOY_R_BUTTON 5
#define PIN_JOY_R_X A2
#define PIN_JOY_R_Y A3


long heartbeat = 0;

void setup() {
  #pragma region OLED init
  /* Select the font to use with menu and all font functions */
  ssd1306_setFixedFont(ssd1306xled_font8x16);
  ssd1306_128x32_i2c_init();
  // ssd1306_128x64_spi_init(-1, 0, 1);  // Use this line for nano pi (RST not used, 0=CE, gpio1=D/C)
  // ssd1306_128x64_spi_init(3,4,5);     // Use this line for Atmega328p (3=RST, 4=CE, 5=D/C)
  // ssd1306_128x64_spi_init(24, 0, 23); // Use this line for Raspberry  (gpio24=RST, 0=CE, gpio23=D/C)
  // ssd1306_128x64_spi_init(22, 5, 21); // Use this line for ESP32 (VSPI)  (gpio22=RST, gpio5=CE for VSPI, gpio21=D/C)
  // composite_video_128x64_mono_init(); // Use this line for ESP32 with Composite video support

  ssd1306_clearScreen();
  ssd1306_printFixed (0,  0, "Initializing...", STYLE_NORMAL);
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
}

void loop() {

  // On joystick button press
  if (digitalRead(PIN_JOY_L_BUTTON) == LOW) {
    // CODE
    data.lButton = true;
    // delay(200); // TODO replace with millis
  } else {
    data.lButton = false;
  }

  // On joystick button press
  if (digitalRead(PIN_JOY_R_BUTTON) == LOW) {
    // CODE
    data.rButton = true;
    // delay(200); // TODO replace with millis
  } else {
    data.rButton = false;
  }

  data.lServo = analogRead(PIN_JOY_L_Y); 
  data.lDir = analogRead(PIN_JOY_L_X);
  data.rServo = analogRead(PIN_JOY_R_Y); 
  data.rDir = analogRead(PIN_JOY_R_X);

  Serial.println("lButt: " + (String)data.lButton + ", lServo: " + (String)data.lServo + ", lDir: " + (String)data.lDir);

  //THIS IS YOUR VARIABALE ALLOCATION BLOCK

  //This is where you will allocate values to the initialized variables in your DataPacket STRUCTure
  //YOU WILL NEED TO DO THIS FOR EACH SENSOR READING
  //TL;DR - read each sensor and save its readings to an initialized variables above
  // data.TransmitTheBool = !data.TransmitTheBool;// change the boolean value of our variable and then assign it to that variable in the data structure
  //saving that  variable into our data variable which is in our DataPacket structure

  //THIS IS the end of YOUR VARIABALE ALLOCATION BLOCK

  // Serial.println(data.TransmitTheBool);//print the variable were transmitting here
  //not necessary but just for funsies
  radio.write(&data, sizeof(DataPacket));//transmit data
  //here we are transmitting the structure 'data' within the strucure 'DataPacket'

  // if (radio.available()) {
  //   radio.read(&backData, sizeof(backDataPacket));
  //   delay(20);
  //   Serial.println("Heartbeat");
  //   heartbeat = millis();
  // }

  // Heartbeating
  // if (millis() - heartbeat > 1000) {
  //   ssd1306_clearScreen();
  //   ssd1306_printFixed (0,  0, "NO COMM", STYLE_NORMAL);
  // } else {
  //   ssd1306_clearScreen();
  //   ssd1306_printFixed (0,  0, "OK", STYLE_NORMAL);
  // }

  delay(20);//chill out for like 20 ms...NEED THIS FOR TRANSMISSION DELAY

}





// static void bitmapDemo()
// {
//     ssd1306_drawBitmap(0, 0, 128, 64, Sova);
//     delay(1000);
//     ssd1306_invertMode();
//     delay(2000);
//     ssd1306_normalMode();
// }

static void spriteDemo()
{
    ssd1306_clearScreen();
    /* Declare variable that represents our sprite */
    SPRITE sprite;
    /* Create sprite at 0,0 position. The function initializes sprite structure. */
    sprite = ssd1306_createSprite( 0, 0, spriteWidth, heartImage );
    for (int i=0; i<250; i++)
    {
        delay(15);
        sprite.x++;
        if (sprite.x >= ssd1306_displayWidth())
        {
            sprite.x = 0;
        }
        sprite.y++;
        if (sprite.y >= ssd1306_displayHeight())
        {
            sprite.y = 0;
        }
        /* Erase sprite on old place. The library knows old position of the sprite. */
        sprite.eraseTrace();
        /* Draw sprite on new place */
        sprite.draw();
    }
}

static void textDemo()
{
    ssd1306_setFixedFont(ssd1306xled_font6x8);
    ssd1306_clearScreen();
    ssd1306_printFixed(0,  8, "Normal text", STYLE_NORMAL);
    ssd1306_printFixed(0, 16, "Bold text", STYLE_BOLD);
    ssd1306_printFixed(0, 24, "Italic text", STYLE_ITALIC);
    ssd1306_negativeMode();
    ssd1306_printFixed(0, 32, "Inverted bold", STYLE_BOLD);
    ssd1306_positiveMode();
    delay(3000);
}

static void canvasDemo()
{
    uint8_t buffer[64*16/8];
    NanoCanvas canvas(64,16, buffer);
    ssd1306_setFixedFont(ssd1306xled_font6x8);
    ssd1306_clearScreen();
    canvas.clear();
    canvas.fillRect(10, 3, 80, 5, 0xFF);
    canvas.blt((ssd1306_displayWidth()-64)/2, 1);
    delay(500);
    canvas.fillRect(50, 1, 60, 15, 0xFF);
    canvas.blt((ssd1306_displayWidth()-64)/2, 1);
    delay(1500);
    canvas.printFixed(20, 1, " DEMO ", STYLE_BOLD );
    canvas.blt((ssd1306_displayWidth()-64)/2, 1);
    delay(3000);
}

static void drawLinesDemo()
{
    ssd1306_clearScreen();
    for (uint8_t y = 0; y < ssd1306_displayHeight(); y += 8)
    {
        ssd1306_drawLine(0,0, ssd1306_displayWidth() -1, y);
    }
    for (uint8_t x = ssd1306_displayWidth() - 1; x > 7; x -= 8)
    {
        ssd1306_drawLine(0,0, x, ssd1306_displayHeight() - 1);
    }
    delay(3000);
}
