// Author: The Gaugfather

#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7); 

const boolean SERIAL_DEBUG = false;

const int FILL_BUTTON = 22;
const int STOP_BUTTON = 24;
const int ON_BUTTON = 26;

const int SOLENOID = 35;

boolean fill_lastButton = LOW;
boolean fill_currentButton = LOW;
boolean is_filling = false;

boolean stop_lastButton = LOW;
boolean stop_currentButton = LOW;
boolean is_stopped = true;

boolean on_lastButton = LOW;
boolean on_currentButton = LOW;
boolean is_on = false;

unsigned long fill_startTime = 0;
unsigned long AUTO_FILL_TIME = 152000; // 2 min 32 sec

unsigned long TIMER = 60000; // 1 min
unsigned long startTime = 0;
unsigned long endTime = 0;

int water_animation2 = 1;
int water_animation2_pic = 0;
int water_animation3 = 2;
int water_animation3_pic = 0;

byte water[8] = {
      0b00000,
      0b00100,
      0b01110,
      0b11111,
      0b11111,
      0b11111,
      0b01110,
      0b00000
};
byte waterEnd[8] = {
      0b00100,
      0b00000,
      0b10001,
      0b00000,
      0b00000,
      0b00100,
      0b01110,
      0b11111
};  
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

void serialDebug(String message){
  if(SERIAL_DEBUG){
    Serial.println(message);
  }
}
void setup() {
  if(SERIAL_DEBUG) {
   Serial.begin(9600); 
  }
  
  pinMode(SOLENOID, OUTPUT);

  lcd.begin(20,4);
  lcd.setBacklightPin(3,POSITIVE);
  setDisplayHeader();
  lcd.createChar(0, water);
  lcd.createChar(1, waterEnd);
  lcd.createChar(2, skull);
  lcd.createChar(3, alien);
}

void setDisplayHeader() {
  lcd.clear();
  lcd.setCursor( 1, 0);
  lcd.write(byte(3));
  lcd.setCursor( 2, 0);
  lcd.print("|GAUG-FILLER|");
  lcd.write(byte(3));
  lcd.setCursor(6,1);
}

void clearLcdPoint(int row, int column) {
  serialDebug("Clear row:" + String(row) + ", column: " + String(column));
  lcd.setCursor(column, row);
  lcd.write(" ");
}

void clearAnimation(int row, int column) {
  if(row != 0) {
    clearLcdPoint(0, column);
  }
  if(row != 1) {
    clearLcdPoint(1, column);
  }
  if(row != 2) {
    clearLcdPoint(2, column);
  }
  if(row != 3) {
    clearLcdPoint(3, column);
  }
  
}

void waterAnimation() {
  water_animation2 = water_animation2 + 1;
  water_animation3 = water_animation3 + 1;
  if(water_animation2 > 4) {
    water_animation2 = 0;
  } 
  if(water_animation2 == 3) {
    water_animation2_pic = 1;
  } else {
    water_animation2_pic = 0;
  }
  if(water_animation3 > 4) {
    water_animation3 = 0;
  }
  if(water_animation3 == 3) {
    water_animation3_pic = 1;
  } else {
    water_animation3_pic = 0;
  }
  clearAnimation(water_animation2, 18);
  clearAnimation(water_animation3, 19);
 
  lcd.setCursor(18, water_animation2);
  lcd.write(byte(water_animation2_pic));
  lcd.setCursor(19, water_animation3);
  lcd.write(byte(water_animation3_pic));
}
void setDisplayFilling() {
  lcd.setCursor(3,1);
}

boolean toggleDisplayOff() {
  is_on = false;
  lcd.setBacklight(LOW);
  startTime = 0;
  endTime = 0;
  setDisplayHeader();
}

boolean toggleDisplayOn() {
  is_on = true;
  lcd.setBacklight(HIGH);
  startTime = millis();
  endTime = millis() + TIMER;
}

boolean debounceButton(boolean last, int button) {
  boolean current = digitalRead(button);
  if(last != current) {
    delay(5); //5ms
    current = digitalRead(button);
  }
  return current;
  
}

void triggerSolenoid() {
  serialDebug("HIGH SOLENOID");
  digitalWrite(SOLENOID, HIGH);
  delay(10000); //10000 is 10 seconds
  serialDebug("LOW SOLENOID");
  digitalWrite(SOLENOID, LOW);
  delay(10000);
}

void fillWater() {
  serialDebug("FILL WATER...");
  setDisplayFilling();
  digitalWrite(SOLENOID, HIGH);
}

void stopFillWater() {
  serialDebug("STOP FILL WATER...");

  clearAnimation(4, 18);
  clearAnimation(4, 19);

  lcd.setCursor(0,2);
  lcd.print("    stopped!     ");  
  lcd.setCursor(0,3);
  lcd.print("      |");
  lcd.write(byte(2));
  lcd.write(byte(2));
  lcd.print("|     ");
  endTime = millis() + TIMER;
  digitalWrite(SOLENOID, LOW);
}

void monitorButtons() {
  on_currentButton = debounceButton(on_lastButton, ON_BUTTON);
  if(!is_on && on_currentButton){
    toggleDisplayOn();
    on_lastButton = LOW;
  }
  if(is_on) {
    monitorFillButton();
  }
}

void monitorStopButton() {
  stop_currentButton = debounceButton(stop_lastButton, STOP_BUTTON);
  if(stop_lastButton == LOW && stop_currentButton == HIGH && (is_filling)) {
    serialDebug("STOPPED FILLING!!");
    is_stopped = true;
    is_filling = false;
    stopFillWater();
  }
  stop_lastButton = stop_currentButton;
}

void monitorFillButton() {
  fill_currentButton = debounceButton(fill_lastButton, FILL_BUTTON);

  if(fill_lastButton == LOW && fill_currentButton == HIGH) { 
    is_stopped = false;
    is_filling = true;
    fill_startTime = millis();
    fillWater();
  }
  while(is_filling && !is_stopped && ((millis() - fill_startTime) <= AUTO_FILL_TIME)) {
    serialDebug("AUTO-FILLING (" + String((millis() - fill_startTime) / 1000) + " seconds of " + String((AUTO_FILL_TIME / 1000)) + " seconds)");
    lcd.setCursor(2,1);
   
    double startTime = (millis() - fill_startTime) / 1000 / (AUTO_FILL_TIME / 1000);
    double num1 = (millis() - fill_startTime) / 1000;
    double num2 = AUTO_FILL_TIME / 1000;
    double num3 = (num1 / num2) * 100;
    int fillPercent = (millis() - fill_startTime) / 1000 / ( AUTO_FILL_TIME / 1000);
    int percent = round(num3);
    serialDebug("FILL PERCENT = " + String(fillPercent) + "%");
    lcd.setCursor(0,2);
    lcd.print("drinking gaug-ter");
    lcd.setCursor(2,3);
    lcd.write(byte(0));
    lcd.write(byte(0));
    lcd.write(byte(0));
    lcd.print(" |" + String(percent) + "%" + "| ");
    lcd.write(byte(0));
    lcd.write(byte(0));
    lcd.write(byte(0));
    waterAnimation();
    monitorStopButton(); 
  }
  if(fill_lastButton == LOW && fill_currentButton == HIGH) {
    stopFillWater();
  }
  if(is_on && is_stopped && (millis() > endTime)) {
    toggleDisplayOff();
  }
  
  fill_lastButton = fill_currentButton; 
  is_filling = false;
  is_stopped = true;
  fill_startTime = 0; 
}

void loop() {
  monitorButtons();
}
