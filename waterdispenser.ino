// Author: The Gaugfather

#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7); 

const boolean SERIAL_DEBUG = true;

// ************************
// * PINS
// ************************
const int FILL_BUTTON = 22;
const int STOP_BUTTON = 24;
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
// * Serial log messages if debug is enabled
// ************************
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
  }
  if(stop_lastButton == LOW && stop_currentButton == HIGH && !is_on) {
    turnLcdDisplayOn();  
  }
  stop_lastButton = stop_currentButton;
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
}
