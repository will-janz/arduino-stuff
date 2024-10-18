// include libraries needed for passive buzzer songs
// #include <TimerFreeTone.h>
// #include <pitches.h>

unsigned long startTime=millis(); // Variable to keep track of when the button is pressed
unsigned long timeLimit=10000; // Total timer is 10 sec long
unsigned long redTime=5000; // LED turns red after 5 sec
unsigned long elapsedTime=0; // Variable to measure elapsed time, used to determine color and buzzer state

bool activate=false; // Toggle variable to determine if timer is active or not

int TONE_PIN=8; // Buzzer pin
int buttonPin=2; // Button connected to this pin
int redPin=6; // RGB LED red pin
int greenPin=5; // RGB LED green pin
int bluePin=3; // RGB LED blue pin


int piezo = 8; // Connect your piezo buzzer to this pin or change  it to match your circuit!
int led = LED_BUILTIN; 

volatile int beatlength  = 100; // determines tempo
float beatseparationconstant = 0.3;

int threshold;

int  a; // part index
int b; // song index
int c; // lyric index




void setup() {
  // Set pin modes
  pinMode(TONE_PIN,OUTPUT);
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
  pinMode(buttonPin,INPUT_PULLUP);

  Serial.begin(9600);


  pinMode(piezo, OUTPUT);
  pinMode(led,  OUTPUT);

  digitalWrite(led, LOW);
  a = 4;
  b = 0;
  c = 0;

}

void loop() {
  if (digitalRead(buttonPin)==LOW){ // When button is pressed
    activate=!activate; // Change state of activate, will reset timer if pressed during countdown
    startTime=millis(); // Set starting time with millis
    delay(250); // debounce delay
  }

  if (!activate){ // If activate is not true, green light on
    // Put code here to make the LED green
    setLED(0, 255, 0);
    Serial.println("Inactive.");
  }
  else { // if activate is true, run the timer program
    elapsedTime = millis() - startTime; // Calculate elapsed time here (time since the button was pressed) 
    Serial.print(elapsedTime);

    if (elapsedTime < 5000) { // If time is not past 5 sec, display yellow 
      // Put code to turn the LED yellow here
      setLED(255, 255, 0);
      Serial.println("Early.");
    }
    else if (elapsedTime >= 5000 && elapsedTime < 10000) { // If time is between 5-10 sec, display red
      // Put code to turn the LED red here
      setLED(255, 0, 0);
      Serial.println("Middle.");
    }
    else if (elapsedTime > 10000 && elapsedTime < 27000){ // If time is between 10-11 sec, red and play buzzer song
      // Put code to turn the LED red and turn the buzzer on here
      setLED(255, 0, 0);
      Serial.println("Gonna play.");
      play();
    }
    else { // Reset after buzzer
    Serial.println("Deactivating.");
      analogWrite(greenPin,255); // LED green
      analogWrite(bluePin,0);
      analogWrite(redPin,0);
      activate=false; // Set toggle variable to false to deactive timer
    }
  }
  
}



// void buzzer_alarm(){
//   float tempo = ; //beats per minute
//   int melody[] = { }; // notes in the melody (as note names or as frequencies)
//   float duration[] = { }; // lengths as number of beats per note in melody
//    // Array items can be multiple bytes. sizeof()/sizeof(arr[0]) gives array length
//   int numNotes = sizeof(melody)/sizeof(melody[0]);
//   Serial.println("Playing Alarm");
//   play_song(tempo, melody, duration, numNotes); // Call provided function with all song details
// }


// // This function can play any song and provide Serial feedback
// // Tempo should be given as beats per minute
// // Melody should be given as an array of note frequences (or names if using pitches library)
// // Duration should be given as an array with number of beats for each melody note
// // numNotes is the number of notes in the song
// void play_song(float tempo, int melody[], float duration[], int numNotes) {
//   float beat_delay = (1/tempo) * 60 * 1000; //ms per beat
//   Serial.print(tempo);
//   Serial.print(" beats per minute -> ");
//   Serial.print(beat_delay);
//   Serial.println(" ms per beat");
//   for (int thisNote = 0; thisNote < numNotes; thisNote++) { // Loop through the notes in the array.
//     Serial.print(thisNote);
//     Serial.print(" ");
//     Serial.print(melody[thisNote]);
//     Serial.print(" Hz, ");
//     Serial.print(duration[thisNote]);
//     Serial.print(" beats, ");
//     Serial.print(duration[thisNote]*beat_delay);
//     Serial.println(" milliseconds");
//     TimerFreeTone(TONE_PIN, melody[thisNote], duration[thisNote]*beat_delay); // Play thisNote for duration
//     delay(50); // Short delay between notes.
//   }   
// }

// Helper functions
void setLED(int r, int g, int b) {
  g = g * 0.35;
  b = b * 0.3;

  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}

#define  a3f    208     // 208 Hz
#define  b3f    233     // 233 Hz
#define  b3     247     // 247 Hz
#define  c4     261     // 261 Hz MIDDLE C
#define  c4s    277     // 277 Hz
#define  e4f    311     // 311 Hz    
#define  f4     349     // 349 Hz 
#define  a4f    415     // 415 Hz  
#define  b4f    466     // 466 Hz 
#define  b4     493     //  493 Hz 
#define  c5     523     // 523 Hz 
#define  c5s    554     // 554  Hz
#define  e5f    622     // 622 Hz  
#define  f5     698     // 698 Hz 
#define  f5s    740     // 740 Hz
#define  a5f    831     // 831 Hz 

#define  rest    -1




// Parts 1 and 2 (Intro)

int song1_intro_melody[] =
{c5s,  e5f, e5f, f5, a5f, f5s, f5, e5f, c5s, e5f, rest, a4f, a4f};

int song1_intro_rhythmn[]  =
{6, 10, 6, 6, 1, 1, 1, 1, 6, 10, 4, 2, 10};

// Parts 3 or 5 (Verse 1)

int  song1_verse1_melody[] =
{ rest, c4s, c4s, c4s, c4s, e4f, rest, c4, b3f, a3f,
  rest, b3f, b3f, c4, c4s, a3f, a4f, a4f, e4f,
  rest, b3f, b3f, c4, c4s, b3f,  c4s, e4f, rest, c4, b3f, b3f, a3f,
  rest, b3f, b3f, c4, c4s, a3f, a3f, e4f,  e4f, e4f, f4, e4f,
  c4s, e4f, f4, c4s, e4f, e4f, e4f, f4, e4f, a3f,
  rest,  b3f, c4, c4s, a3f, rest, e4f, f4, e4f
};

int song1_verse1_rhythmn[] =
{  2, 1, 1, 1, 1, 2, 1, 1, 1, 5,
  1, 1, 1, 1, 3, 1, 2, 1, 5,
  1, 1, 1, 1, 1,  1, 1, 2, 1, 1, 1, 1, 3,
  1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 4,
  5, 1, 1, 1,  1, 1, 1, 1, 2, 2,
  2, 1, 1, 1, 3, 1, 1, 1, 3
};

const char* lyrics_verse1[]  =
{ "We're ", "no ", "strangers ", "", "to ", "love ", "", "\
\
",
  "You ", "know ", "the ", "rules ", "and ", "so ", "do ", "I\
\
",
  "A ", "full ", "commitment's ", "", "", "what ", "I'm ", "thinking  ", "", "of", "\
\
",
  "You ", "wouldn't ", "", "get ", "this  ", "from ", "any ", "", "other ", "", "guy\
\
",
  "I ", "just  ", "wanna ", "", "tell ", "you ", "how ", "I'm ", "feeling", "\
\
",
  "Gotta ", "", "make ", "you ", "understand", "", "\
\
"
};

//  Parts 4 or 6 (Chorus)

int song1_chorus_melody[] =
{ b4f, b4f, a4f, a4f,
  f5, f5, e5f, b4f, b4f, a4f, a4f, e5f, e5f, c5s, c5, b4f,
  c5s, c5s, c5s, c5s,
  c5s, e5f, c5, b4f, a4f, a4f, a4f, e5f, c5s,
  b4f, b4f, a4f, a4f,
  f5,  f5, e5f, b4f, b4f, a4f, a4f, a5f, c5, c5s, c5, b4f,
  c5s, c5s, c5s, c5s,
  c5s, e5f, c5, b4f, a4f, rest, a4f, e5f, c5s, rest
};

int song1_chorus_rhythmn[]  =
{ 1, 1, 1, 1,
  3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
  1, 1, 1, 1,
  3, 3, 3, 1, 2, 2, 2, 4, 8,
  1, 1, 1, 1,
  3, 3, 6, 1, 1, 1, 1, 3, 3, 3,  1, 2,
  1, 1, 1, 1,
  3, 3, 3, 1, 2, 2, 2, 4, 8, 4
};

const char*  lyrics_chorus[] =
{ "Never ", "", "gonna ", "", "give ", "you ",  "up\
\
",
  "Never ", "", "gonna ", "", "let ", "you ", "down",  "", "\
\
",
  "Never ", "", "gonna ", "", "run ", "around ",  "", "", "", "and ", "desert ", "", "you\
\
",
  "Never ", "",  "gonna ", "", "make ", "you ", "cry\
\
",
  "Never ", "", "gonna  ", "", "say ", "goodbye ", "", "", "\
\
",
  "Never ", "",  "gonna ", "", "tell ", "a ", "lie ", "", "", "and ", "hurt ",  "you\
\
"
};


void  play() {
  int notelength;
  if (a == 1 || a == 2) {
    // intro
    notelength  = beatlength * song1_intro_rhythmn[b];
    if (song1_intro_melody[b] > 0) {
      digitalWrite(led, HIGH);
      tone(piezo, song1_intro_melody[b], notelength);
    }
    b++;
    if (b >= sizeof(song1_intro_melody) / sizeof(int)) {
      a++;
      b = 0;
      c = 0;
    }
  }
  else if (a == 3  || a == 5) {
    // verse
    notelength = beatlength * 2 * song1_verse1_rhythmn[b];
    if (song1_verse1_melody[b] > 0) {
      digitalWrite(led, HIGH);
      Serial.print(lyrics_verse1[c]);
      tone(piezo, song1_verse1_melody[b], notelength);
      c++;
    }
    b++;
    if (b >= sizeof(song1_verse1_melody) / sizeof(int)) {
      a++;
      b = 0;
      c = 0;
    }
  }
  else if (a == 4 || a == 6) {
    // chorus
    notelength = beatlength * song1_chorus_rhythmn[b];
    if  (song1_chorus_melody[b] > 0) {
      digitalWrite(led, HIGH);
      Serial.print(lyrics_chorus[c]);
      tone(piezo, song1_chorus_melody[b], notelength);
      c++;
    }
    b++;
    if (b >= sizeof(song1_chorus_melody) / sizeof(int)) {
      Serial.println("");
      a++;
      b = 0;
      c = 0;
    }
  }
  delay(notelength);
  noTone(piezo);
  digitalWrite(led, LOW);
  delay(notelength * beatseparationconstant);
  if (a == 7) { // loop back around to beginning of song
    a = 1;
  }
}