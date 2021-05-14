// Author: The Gaugfather

const int FILL_BUTTON = 22;
const int STOP_BUTTON = 24;
const int AUTO_BUTTON = 26;

const int SOLENOID = 35;

boolean fill_lastButton = LOW;
boolean fill_currentButton = LOW;
boolean is_filling = false;

boolean stop_lastButton = LOW;
boolean stop_currentButton = LOW;
boolean is_stopped = true;

boolean auto_lastButton = LOW;
boolean auto_currentButton = LOW;
boolean auto_filling = false;
unsigned long auto_startTime = 0;
//unsigned long AUTO_FILL_TIME = 10000; // 10 sec
unsigned long AUTO_FILL_TIME = 5000; // 5 sec

void setup() {
   Serial.begin(9600);

   pinMode(SOLENOID, OUTPUT);
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

  //delay(3000);
  Serial.println("HIGH SOLENOID");
  digitalWrite(SOLENOID, HIGH);
  delay(10000); //10000 is 10 seconds
  Serial.println("LOW SOLENOID");
  digitalWrite(SOLENOID, LOW);
  delay(10000);
}

void fillWater() {
  Serial.println("FILL WATER...");
  digitalWrite(SOLENOID, HIGH);
}

void stopFillWater() {
  Serial.println("STOP FILL WATER...");
  digitalWrite(SOLENOID, LOW);
}

void monitorManualFillButton() {
  fill_currentButton = debounceButton(fill_lastButton, FILL_BUTTON);
  if(fill_lastButton == LOW && fill_currentButton == HIGH && is_stopped) {
    is_filling = true;
    is_stopped = false;
    fillWater();
  }
  fill_lastButton = fill_currentButton;
  while(is_filling && !is_stopped) {
    Serial.println("MANUAL-FILLING...");
    monitorStopButton();
  }
}

void monitorStopButton() {
  stop_currentButton = debounceButton(stop_lastButton, STOP_BUTTON);
  if(stop_lastButton == LOW && stop_currentButton == HIGH && (is_filling || auto_filling)) {
    Serial.println("STOPPED FILLING!!");
    is_stopped = true;
    is_filling = false;
    auto_filling = false;
    stopFillWater();
  }
  stop_lastButton = stop_currentButton;
}

void monitorAutoFillButton() {
  auto_currentButton = debounceButton(auto_lastButton, AUTO_BUTTON);
  if(auto_lastButton == LOW && auto_currentButton == HIGH) { 
    is_stopped = false;
    auto_filling = true;
    auto_startTime = millis();
    fillWater();
  }
  while(auto_filling && !is_stopped && ((millis() - auto_startTime) <= AUTO_FILL_TIME)) {
    Serial.println("AUTO-FILLING (" + String((millis() - auto_startTime) / 1000) + " of 10 seconds)");
    monitorStopButton(); 
  }
  if(auto_lastButton == LOW && auto_currentButton == HIGH) {
    stopFillWater();
  }
  
  auto_lastButton = auto_currentButton; 
  auto_filling = false;
  is_stopped = true;
  auto_startTime = 0; 
}

void loop() {
  monitorManualFillButton();
  monitorAutoFillButton();
  //triggerSolenoid();
}
