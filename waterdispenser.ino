// Author: The Gaugfather

#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7); 

const boolean SERIAL_DEBUG = true;


// ************************
// * SOUNDS
// ************************
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0


// change this to whichever pin you want to use
int buzzer = 41;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int gotMelody[] = {

  // Game of Thrones
  // Score available at https://musescore.com/user/8407786/scores/2156716

  NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16, //1
  NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16,
  NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16,
  NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16,
  NOTE_G4,-4, NOTE_C4,-4,//5

  NOTE_DS4,16, NOTE_F4,16, NOTE_G4,4, NOTE_C4,4, NOTE_DS4,16, NOTE_F4,16, //6
  NOTE_D4,-1, //7 and 8
  NOTE_F4,-4, NOTE_AS3,-4,
  NOTE_DS4,16, NOTE_D4,16, NOTE_F4,4, NOTE_AS3,-4,
  NOTE_DS4,16, NOTE_D4,16, NOTE_C4,-1, //11 and 12

  //repeats from 5
  NOTE_G4,-4, NOTE_C4,-4,//5

  NOTE_DS4,16, NOTE_F4,16, NOTE_G4,4, NOTE_C4,4, NOTE_DS4,16, NOTE_F4,16, //6
  NOTE_D4,-1, //7 and 8
  NOTE_F4,-4, NOTE_AS3,-4,
  NOTE_DS4,16, NOTE_D4,16, NOTE_F4,4, NOTE_AS3,-4,
  NOTE_DS4,16, NOTE_D4,16, NOTE_C4,-1, //11 and 12
  NOTE_G4,-4, NOTE_C4,-4,
  NOTE_DS4,16, NOTE_F4,16, NOTE_G4,4,  NOTE_C4,4, NOTE_DS4,16, NOTE_F4,16,

  NOTE_D4,-2,//15
  NOTE_F4,-4, NOTE_AS3,-4,
  NOTE_D4,-8, NOTE_DS4,-8, NOTE_D4,-8, NOTE_AS3,-8,
  NOTE_C4,-1,
  NOTE_C5,-2,
  NOTE_AS4,-2,
  NOTE_C4,-2,
  NOTE_G4,-2,
  NOTE_DS4,-2,
  NOTE_DS4,-4, NOTE_F4,-4, 
  NOTE_G4,-1,
  
  NOTE_C5,-2,//28
  NOTE_AS4,-2,
  NOTE_C4,-2,
  NOTE_G4,-2, 
  NOTE_DS4,-2,
  NOTE_DS4,-4, NOTE_D4,-4,
  NOTE_C5,8, NOTE_G4,8, NOTE_GS4,16, NOTE_AS4,16, NOTE_C5,8, NOTE_G4,8, NOTE_GS4,16, NOTE_AS4,16,
  NOTE_C5,8, NOTE_G4,8, NOTE_GS4,16, NOTE_AS4,16, NOTE_C5,8, NOTE_G4,8, NOTE_GS4,16, NOTE_AS4,16,
  
  REST,4, NOTE_GS5,16, NOTE_AS5,16, NOTE_C6,8, NOTE_G5,8, NOTE_GS5,16, NOTE_AS5,16,
  NOTE_C6,8, NOTE_G5,16, NOTE_GS5,16, NOTE_AS5,16, NOTE_C6,8, NOTE_G5,8, NOTE_GS5,16, NOTE_AS5,16,  
};

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int superMarioMelody[] = {

  // Super Mario Bros theme
  // Score available at https://musescore.com/user/2123/scores/2145
  // Theme by Koji Kondo
  
  
  NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,8, //1
  NOTE_G5,4, REST,4, NOTE_G4,8, REST,4, 
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // 3
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4,NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // repeats from 3
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4,NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,

  
  REST,4, NOTE_G5,8, NOTE_FS5,8, NOTE_F5,8, NOTE_DS5,4, NOTE_E5,8,//7
  REST,8, NOTE_GS4,8, NOTE_A4,8, NOTE_C4,8, REST,8, NOTE_A4,8, NOTE_C5,8, NOTE_D5,8,
  REST,4, NOTE_DS5,4, REST,8, NOTE_D5,-4,
  NOTE_C5,2, REST,2,

  REST,4, NOTE_G5,8, NOTE_FS5,8, NOTE_F5,8, NOTE_DS5,4, NOTE_E5,8,//repeats from 7
  REST,8, NOTE_GS4,8, NOTE_A4,8, NOTE_C4,8, REST,8, NOTE_A4,8, NOTE_C5,8, NOTE_D5,8,
  REST,4, NOTE_DS5,4, REST,8, NOTE_D5,-4,
  NOTE_C5,2, REST,2,

  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,4,//11
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2,

  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,8, NOTE_E5,8,//13
  REST,1, 
  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,4,
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2,
  NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,4,
  NOTE_G5,4, REST,4, NOTE_G4,4, REST,4, 
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // 19
  
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4, NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,

  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // repeats from 19
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4, NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,

  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,//23
  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
  NOTE_D5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_G5,-8, NOTE_F5,-8,
  
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2, //26
  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,
  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
  NOTE_B4,8, NOTE_F5,4, NOTE_F5,8, NOTE_F5,-8, NOTE_E5,-8, NOTE_D5,-8,
  NOTE_C5,8, NOTE_E4,4, NOTE_E4,8, NOTE_C4,2,

  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,//repeats from 23
  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
  NOTE_D5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_G5,-8, NOTE_F5,-8,
  
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2, //26
  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,
  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
  NOTE_B4,8, NOTE_F5,4, NOTE_F5,8, NOTE_F5,-8, NOTE_E5,-8, NOTE_D5,-8,
  NOTE_C5,8, NOTE_E4,4, NOTE_E4,8, NOTE_C4,2,
  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,8, NOTE_E5,8,
  REST,1,

  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,4, //33
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2,
  NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,4,
  NOTE_G5,4, REST,4, NOTE_G4,4, REST,4, 
  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,
  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
  NOTE_D5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_G5,-8, NOTE_F5,-8,
  
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2, //40
  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,
  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
  NOTE_B4,8, NOTE_F5,4, NOTE_F5,8, NOTE_F5,-8, NOTE_E5,-8, NOTE_D5,-8,
  NOTE_C5,8, NOTE_E4,4, NOTE_E4,8, NOTE_C4,2,
  
  //game over sound
  NOTE_C5,-4, NOTE_G4,-4, NOTE_E4,4, //45
  NOTE_A4,-8, NOTE_B4,-8, NOTE_A4,-8, NOTE_GS4,-8, NOTE_AS4,-8, NOTE_GS4,-8,
  NOTE_G4,8, NOTE_D4,8, NOTE_E4,-2,  

};




// ************************
// * PINS
// ************************
const int FILL_BUTTON = 22;
const int STOP_BUTTON = 24;
const int SONG_BUTTON = 26;
const int SOLENOID = 35;


// ************************
// * ON status
// ************************
boolean is_on = false;
unsigned long DISPLAY_TIMER = 60000; // 1 min
unsigned long display_startTime = 0;
unsigned long display_endTime = 0;


// ************************
// * FILL BUTTON
// ************************
boolean fill_lastButton = LOW;
boolean fill_currentButton = LOW;
boolean is_filling = false;
String randomKid = "";

unsigned long AUTO_FILL_TIME = 152000; // 2 min 32 sec
unsigned long fill_startTime = 0;

int droplet_col_1_animation_row = 0;
int droplet_col_2_animation_row = 2;

int droplet_col_1_animation_picture = 0;
int droplet_col_2_animation_picture = 0;

 
// ************************
// * STOP BUTTON
// ************************
boolean stop_lastButton = LOW;
boolean stop_currentButton = LOW;
boolean is_stopped = true;


// ************************
// * SONG BUTTON
// ************************
boolean song_lastButton = LOW;
boolean song_currentButton = LOW;
boolean is_got_playing = false;
boolean is_mario_playing = false;
int song = 0;


void serialDebug(String message){
  if(SERIAL_DEBUG){
    Serial.println(message);
  }
}


// ************************
// * Setup LCD display and emojis
// ************************
void setupLcdDisplay() {
  lcd.begin(20,4);
  lcd.setBacklightPin(3, POSITIVE);

  byte water_droplet[8] = {
      0b00000,
      0b00100,
      0b01110,
      0b11111,
      0b11111,
      0b11111,
      0b01110,
      0b00000
  };
  lcd.createChar(0, water_droplet);
  
  byte water_droplet_splash[8] = {
      0b00100,
      0b00000,
      0b10001,
      0b00000,
      0b00000,
      0b00100,
      0b01110,
      0b11111
  };
  lcd.createChar(1, water_droplet_splash);

  byte skull[8] = {
      0b00000,
      0b01110,
      0b10101,
      0b11011,
      0b01110,
      0b01110,
      0b00000,
      0b00000
  };
  lcd.createChar(2, skull);
  
  byte alien[8] = {
      0b11111,
      0b10101,
      0b11111,
      0b11111,
      0b01110,
      0b01010,
      0b11011,
      0b00000
  };
  lcd.createChar(3, alien);

  setLcdDisplayHeader();   
}


// ************************
// * LCD display Header
// ************************
void setLcdDisplayHeader() {
  lcd.clear();
  lcd.setCursor( 1, 0);
  lcd.write(byte(3));
  lcd.setCursor( 2, 0);
  lcd.print("|GAUG-FILLER|");
  lcd.write(byte(3));
  lcd.setCursor(6,1);
}


// ************************
// * Turns on (brightens) LCD Display
// ************************
boolean turnLcdDisplayOn() {
  serialDebug("Turning On!");
  is_on = true;
  lcd.setBacklight(HIGH);
  display_startTime = millis();
  display_endTime = millis() + DISPLAY_TIMER;
}


// ************************
// * Turns off (dims) LCD Display
// ************************
boolean turnLcdDisplayOff() {
  is_on = false;
  lcd.setBacklight(LOW);
  display_startTime = 0;
  display_endTime = 0;
  setLcdDisplayHeader();
}


// ************************
// * Debounce button reads
// ************************
boolean debounceButton(boolean last, int button) {
  boolean current = digitalRead(button);
  if(last != current) {
    delay(5); // 5 ms
    current = digitalRead(button);
  }
  return current;
}


// ************************
// * LCD display Filling
// ************************
void setLcdDisplayFilling(int percent) {
  lcd.setCursor(0,2);
  lcd.print("drinking " + randomKid);
  lcd.setCursor(2,3);
  lcd.write(byte(0));
  lcd.write(byte(0));
  lcd.write(byte(0));
  lcd.print(" |" + String(percent) + "%" + "| ");
  lcd.write(byte(0));
  lcd.write(byte(0));
  lcd.write(byte(0));

  droplet_col_1_animation_row = lcdDisplayFilling_getWaterAnimationRow(droplet_col_1_animation_row);
  droplet_col_1_animation_picture = lcdDisplayFilling_getWaterAnimationPicture(droplet_col_1_animation_row);
  lcdDisplayFilling_clearAnimation(18, droplet_col_1_animation_row);
  lcdDisplayFilling_printAnimation(18, droplet_col_1_animation_row, droplet_col_1_animation_picture);
  
  droplet_col_2_animation_row = lcdDisplayFilling_getWaterAnimationRow(droplet_col_2_animation_row);
  droplet_col_2_animation_picture = lcdDisplayFilling_getWaterAnimationPicture(droplet_col_2_animation_row);
  lcdDisplayFilling_clearAnimation(19, droplet_col_2_animation_row);
  lcdDisplayFilling_printAnimation(19, droplet_col_2_animation_row, droplet_col_2_animation_picture);
}

int lcdDisplayFilling_getWaterAnimationRow(int dropletRow) {
  dropletRow = dropletRow + 1;
  if(dropletRow == 4) {
    dropletRow = 0;
  }
  return dropletRow;
}

int lcdDisplayFilling_getWaterAnimationPicture(int dropletRow) {
  int dropletPicture = 0;
  if(dropletRow == 3) {
    dropletPicture = 1;
  }
  return dropletPicture;
}

void lcdDisplayFilling_clearAnimation(int column, int row) {
  if(row != 0) {
    lcdDisplayFilling_clearLcdPoint(0, column);
  }
  if(row != 1) {
    lcdDisplayFilling_clearLcdPoint(1, column);
  }
  if(row != 2) {
    lcdDisplayFilling_clearLcdPoint(2, column);
  }
  if(row != 3) {
    lcdDisplayFilling_clearLcdPoint(3, column);
  }
  
}

void lcdDisplayFilling_clearLcdPoint(int row, int column) {
  serialDebug("Clear row:" + String(row) + ", column: " + String(column));  
  lcd.setCursor(column, row);

  unsigned long startTime = millis();
  unsigned long endTime = startTime;
  
  // delay between water droplet animation 20ms
  while ((endTime - startTime) <=20) { 
    monitorStopButton();
    endTime = millis();
  }
  
  lcd.write(" ");
}

void lcdDisplayFilling_printAnimation(int column, int row, int pic) {
  lcd.setCursor(column, row);
  lcd.write(byte(pic));
}


// ************************
// * Monitors Fill Button Press
// ************************
void monitorFillButton() {
  fill_currentButton = debounceButton(fill_lastButton, FILL_BUTTON);

  if(is_on && fill_lastButton == LOW && fill_currentButton == HIGH) { 
    is_stopped = false;
    is_filling = true;
    fill_startTime = millis();

    serialDebug("FILLING WATER...");
    randomKid = random(2) ? "liam" : "haddie";  
    digitalWrite(SOLENOID, HIGH);
  }
  while(is_filling && !is_stopped && ((millis() - fill_startTime) <= AUTO_FILL_TIME)) {
    serialDebug("AUTO-FILLING (" + String((millis() - fill_startTime) / 1000) + " seconds of " + String((AUTO_FILL_TIME / 1000)) + " seconds)");    
    double startTime = (millis() - fill_startTime) / 1000 / (AUTO_FILL_TIME / 1000);
    double num1 = (millis() - fill_startTime) / 1000;
    double num2 = AUTO_FILL_TIME / 1000;
    double num3 = (num1 / num2) * 100;
    int fillPercent = (millis() - fill_startTime) / 1000 / ( AUTO_FILL_TIME / 1000);
    int percent = round(num3);
    serialDebug("FILL PERCENT = " + String(fillPercent) + "%");
    
    setLcdDisplayFilling(percent);
    monitorStopButton(); 
  }
  if(is_on && fill_lastButton == LOW && fill_currentButton == HIGH) {
    setLcdDisplayStopped();
    digitalWrite(SOLENOID, LOW);
  }
  if(is_on && is_stopped && (millis() > display_endTime)) {
    turnLcdDisplayOff();
  }
  if(fill_lastButton == LOW && fill_currentButton == HIGH && !is_on) {
    turnLcdDisplayOn();  
  }
  
  fill_lastButton = fill_currentButton; 
  is_filling = false;
  is_stopped = true;
  fill_startTime = 0; 
}


// ************************
// * LCD Display Stopped
// ************************
void setLcdDisplayStopped() {
  serialDebug("STOPPED FILLING WATER...");

  lcdDisplayFilling_clearAnimation(18, 4);
  lcdDisplayFilling_clearAnimation(19, 4);

  lcd.setCursor(0,2);
  lcd.print("    stopped!     ");  
  lcd.setCursor(0,3);
  lcd.print("      |");
  lcd.write(byte(2));
  lcd.write(byte(2));
  lcd.print("|     ");
  display_endTime = millis() + DISPLAY_TIMER;
}


// ************************
// * Play Buzzer Super Mario
// ************************
void playSuperMario() {
  // change this to make the song slower or faster
  int tempo = 200;
  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / tempo;

  int divider = 0, noteDuration = 0;
  // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
  // there are two values per note (pitch and duration), so for each note there are four bytes
  int notes = sizeof(superMarioMelody) / sizeof(superMarioMelody[0]) / 2;

  is_mario_playing = true;

  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    monitorStopButton();
    if(!is_mario_playing) {
      break;
    }
    // calculates the duration of each note
    divider = superMarioMelody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, superMarioMelody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}

// ************************
// * Play Buzzer Game of Thrones Theme
// ************************
void playGameOfThronesTheme(boolean fullSong) {

  // change this to make the song slower or faster
  int tempo = 85;
  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / tempo;

  int divider = 0, noteDuration = 0;
  // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
  // there are two values per note (pitch and duration), so for each note there are four bytes
  int notes = sizeof(gotMelody) / sizeof(gotMelody[0]) / 2;
  if(!fullSong) {
    notes = 6;
  }
  is_got_playing = true;
    
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    serialDebug("Playing note=" + String(thisNote));
    monitorStopButton();
    if(!is_got_playing) {
      break;
    }

    // calculates the duration of each note
    divider = gotMelody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, gotMelody[thisNote], noteDuration * 0.9);
    // Wait for the duration before playing the next note.
    delay(noteDuration);
    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
  is_got_playing = false;
}


// ************************
// * Monitors Stop Button Press
// ************************
void monitorStopButton() {
  stop_currentButton = debounceButton(stop_lastButton, STOP_BUTTON);
  if(stop_lastButton == LOW && stop_currentButton == HIGH && is_filling) {
    serialDebug("STOPPED FILLING!!");
    is_stopped = true;
    is_filling = false;
    setLcdDisplayStopped();
    digitalWrite(SOLENOID, LOW);
    playGameOfThronesTheme(false);
  }
  if(stop_lastButton == LOW && stop_currentButton == HIGH && !is_on) {
    turnLcdDisplayOn();  
  }
  if(stop_lastButton == LOW && stop_currentButton == HIGH && is_got_playing) {
    is_got_playing = false;
  }
  if(stop_lastButton == LOW && stop_currentButton == HIGH && is_mario_playing) {
    is_mario_playing = false;
  }
  stop_lastButton = stop_currentButton;
}

// ************************
// * Monitors Song Button Press
// ************************
void monitorSongButton() {
  song_currentButton = debounceButton(song_lastButton, SONG_BUTTON);
  if(song_lastButton == LOW && song_currentButton == HIGH && !is_filling && is_stopped && song == 0 && !is_got_playing) {
    serialDebug("BOTH BUTTON PRESS - PLAY GOT!!");
    turnLcdDisplayOn();
    playGameOfThronesTheme(true);
    song = 1;
  }
  if(song_lastButton == LOW && song_currentButton == HIGH && !is_filling && is_stopped && song == 1 && !is_mario_playing) {
    serialDebug("BOTH BUTTON PRESS - PLAY GOT!!");
    turnLcdDisplayOn();
    playSuperMario();
    song = 0;
  }
  song_lastButton = song_currentButton;
}

// ************************
// * Arduino MAIN SETUP
// ************************
void setup() {
  if(SERIAL_DEBUG) {
   Serial.begin(9600); 
  }
  pinMode(SOLENOID, OUTPUT);
 
  setupLcdDisplay();
}


// ************************
// * Arduino MAIN LOOP
// ************************
void loop() {
  monitorStopButton();
  monitorFillButton();
  monitorSongButton();
}
