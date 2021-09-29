// Author: The Gaugfather

#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7); 

#include "music_melodies.h"

const boolean SERIAL_DEBUG = true;






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
boolean is_filling_coffee = false;
String randomKid = "";

unsigned long AUTO_FILL_TIME = 153000; // (gallon) 2 min 33 sec
unsigned long COFFEE_FILL_TIME = 121000; // (quart) 1 min 30 sec
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
boolean is_lion_sleeps_tonight_playing = false;
boolean is_take_on_me_playing = false;
boolean is_never_gonna_give_playing = false;
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

  byte speaker[8] = {
      0b00001,
      0b00011,
      0b01111,
      0b01111,
      0b01111,
      0b00011,
      0b00001,
      0b00000
  };
  lcd.createChar(4, speaker);

  byte sound[8] = {
      0b00001,
      0b00011,
      0b00101,
      0b01001,
      0b01001,
      0b01011,
      0b11011,
      0b11000
  };
  lcd.createChar(5, sound);

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
    delay(4);
    current = digitalRead(button);
  }
  return current;
}


// ************************
// * LCD display Filling
// ************************
void setLcdDisplayFilling(int percent, boolean is_auto) {
  lcd.setCursor(0,2);
  if(is_auto) {
    lcd.print("drinking " + randomKid);
  } else {
    lcd.print("coffee 4 " + randomKid);
  }
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
// * Display fill container percentage
// ************************
void showFillContainerPercentage(String type, unsigned long fill_time) {
  serialDebug(String(type) + " (" + String((millis() - fill_startTime) / 1000) + " seconds of " + String((fill_time / 1000)) + " seconds)");    
  double startTime = (millis() - fill_startTime) / 1000 / (fill_time / 1000);
  double num1 = (millis() - fill_startTime) / 1000;
  double num2 = fill_time / 1000;
  double num3 = (num1 / num2) * 100;
  int fillPercent = (millis() - fill_startTime) / 1000 / ( fill_time / 1000);
  int percent = round(num3);
  serialDebug(String(type) + " PERCENT = " + String(fillPercent) + "%");

  boolean auto_fill = true;
  if(type == "COFFEE-FILL") {
    auto_fill = false;
  }
  setLcdDisplayFilling(percent, auto_fill);
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
    showFillContainerPercentage("AUTO-FILL", AUTO_FILL_TIME);
    
    monitorStopButton(); 
    monitorSongButton();
  }
  if(is_on && fill_lastButton == LOW && fill_currentButton == HIGH) {
    setLcdDisplayStopped();
    digitalWrite(SOLENOID, LOW);
    playMelody(0, false);
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
// * Display Music Track
// ************************
void setLcdDisplayMusicPlaying(int music_track) {
  serialDebug("Displaying Music Track: " + String(music_track));
  String song;
  switch(music_track) {
    case 0:
      song = "Game of Thrones ";
      break;
    case 1:
      song = "Super Mario Bro.";
      break;
    case 2:
      song = "Lion Sleeps Ton.";
      break;
    case 3:
      song = "  Take on Me    ";
      break;
    case 4:
      song = "Never Gonna Give";
      break;
  }
  lcd.setCursor(1,2);
  lcd.write(byte(4));
  lcd.print("   dancing to  ");
  lcd.setCursor(18,2);
  lcd.write(byte(5));
  lcd.setCursor(1,3);
  lcd.print(song);
}


// ************************
// * Play a Music Melody
// ************************
void playMelody(int music_track, boolean full_song) {
  int tempo;
  int notes;
  int* melody;
  setLcdDisplayMusicPlaying(music_track);
  switch(music_track) {
    case 0:
      tempo = 85;
      melody = gotMelody;
      notes = sizeof(gotMelody) / sizeof(gotMelody[0]) / 2;
      break;
    case 1:
      tempo = 200;
      melody = superMarioMelody;
      notes = sizeof(superMarioMelody) / sizeof(superMarioMelody[0]) / 2;
      break;
    case 2: 
      tempo = 122;
      melody = lionSleepsTonightMelody;
      notes = sizeof(lionSleepsTonightMelody) / sizeof(lionSleepsTonightMelody[0]) / 2;
      break;
    case 3: 
      tempo = 140;
      melody = takeOnMeMelody;
      notes = sizeof(takeOnMeMelody) / sizeof(takeOnMeMelody[0]) / 2;
      break;
    case 4: 
      tempo = 144;
      melody = neverGonnaGiveYouUpMelody;
      notes = sizeof(neverGonnaGiveYouUpMelody) / sizeof(neverGonnaGiveYouUpMelody[0]) / 2;
      break;
  }
  if(!full_song) {
    notes = 6;
  }
  int wholenote = (60000 * 4) / tempo;
  int divider = 0, noteDuration = 0;
  
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    monitorStopButton();
    boolean break_loop = false;
    if((!is_got_playing && music_track == 0) || 
      (!is_mario_playing && music_track == 1) || 
      (!is_lion_sleeps_tonight_playing && music_track == 2) ||
      (!is_take_on_me_playing && music_track == 3) ||
      (!is_never_gonna_give_playing && music_track == 4)
      ) {
      break;
    }
    divider = melody[thisNote + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }

    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    delay(noteDuration);

    noTone(buzzer);
  }
  
}


// ************************
// * Monitors Stop Button Press
// ************************
void monitorStopButton() {
  stop_currentButton = debounceButton(stop_lastButton, STOP_BUTTON);
  if(stop_currentButton == HIGH) {
    serialDebug("stop pressed");
  }
  if(stop_lastButton == LOW && stop_currentButton == HIGH && is_filling) {
    serialDebug("STOPPED FILLING!!");
    is_stopped = true;
    is_filling = false;
    setLcdDisplayStopped();
    digitalWrite(SOLENOID, LOW);
    playMelody(0, false);
  }
  if(stop_lastButton == LOW && stop_currentButton == HIGH && !is_on) {
    turnLcdDisplayOn();  
  }
  if(stop_lastButton == LOW && stop_currentButton == HIGH) {
    if(is_got_playing) {
      is_got_playing = false;
    }
    if(is_mario_playing) {
      is_mario_playing = false;
    }
    if(is_lion_sleeps_tonight_playing) {
      is_lion_sleeps_tonight_playing = false;
    }
    if(is_take_on_me_playing) {
      is_take_on_me_playing = false;
    }
    if(is_never_gonna_give_playing) {
      is_never_gonna_give_playing = false;
    }
  }
  stop_lastButton = stop_currentButton;
}

// ************************
// * Monitors Song Button Press or Coffee Container Button Press
// ************************
void monitorSongButton() {
  song_currentButton = debounceButton(song_lastButton, SONG_BUTTON);
  if(song_lastButton == LOW && song_currentButton == HIGH && !is_filling && is_stopped && song == 0 && !is_got_playing) {
    is_got_playing = true;
    turnLcdDisplayOn();
    playMelody(0, true);
    song = 1;
  }
  if(song_lastButton == LOW && song_currentButton == HIGH && !is_filling && is_stopped && song == 1 && !is_got_playing && !is_mario_playing) {
    is_mario_playing = true;
    turnLcdDisplayOn();
    playMelody(1, true);  // play super mario bros
    song = 2;
  }
  if(song_lastButton == LOW && song_currentButton == HIGH && !is_filling && is_stopped && song == 2 && !is_mario_playing && !is_lion_sleeps_tonight_playing) {
    is_lion_sleeps_tonight_playing = true;
    turnLcdDisplayOn();
    playMelody(2, true);  // play lion sleeps tonight
    song = 3;
  }
  if(song_lastButton == LOW && song_currentButton == HIGH && !is_filling && is_stopped && song == 3 && !is_lion_sleeps_tonight_playing && !is_take_on_me_playing) {
    is_take_on_me_playing = true;
    turnLcdDisplayOn();
    playMelody(3, true);  // play take on me
    song = 4;
  }
  if(song_lastButton == LOW && song_currentButton == HIGH && !is_filling && is_stopped && song == 4 && !is_take_on_me_playing && !is_never_gonna_give_playing) {
    is_never_gonna_give_playing = true;
    turnLcdDisplayOn();
    playMelody(4, true);  // play never gonna give
    song = 0; //reset
  }
  if(song_lastButton == LOW && song_currentButton == HIGH && is_filling && !is_filling_coffee) {
    // Fill Coffee Container
    is_filling_coffee = true;
    while(is_filling && !is_stopped && is_filling_coffee && ((millis() - fill_startTime) <= COFFEE_FILL_TIME)) {
      showFillContainerPercentage("COFFEE-FILL", COFFEE_FILL_TIME);
      
      monitorStopButton(); 
    }
    is_filling_coffee = false;
    fill_startTime = AUTO_FILL_TIME;
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
