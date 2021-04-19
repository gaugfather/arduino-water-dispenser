// Author: The Gaugfather

const int FILL_BUTTON = 22;
const int STOP_BUTTON = 24;

boolean fill_lastButton = LOW;
boolean fill_currentButton = LOW;
boolean is_filling = false;

boolean stop_lastButton = LOW;
boolean stop_currentButton = LOW;
boolean is_stopped = true;

void setup() {
   Serial.begin(9600);
}

boolean debounceButton(boolean last, int button) {
  boolean current = digitalRead(button);
  if(last != current) {
    delay(5); //5ms
    current = digitalRead(button);
  }
  return current;
  
}

void monitorFillButton() {
  fill_currentButton = debounceButton(fill_lastButton, FILL_BUTTON);
  if(fill_lastButton == LOW && fill_currentButton == HIGH && is_stopped) {
    //Serial.println("FILLING...");
    is_filling = true;
    is_stopped = false;
  }
  fill_lastButton = fill_currentButton;
  while(is_filling && !is_stopped) {
    Serial.println("FILLING...");
    monitorStopButton();
  }
}

void monitorStopButton() {
  stop_currentButton = debounceButton(stop_lastButton, STOP_BUTTON);
  if(stop_lastButton == LOW && stop_currentButton == HIGH && is_filling) {
    Serial.println("STOPPED!");
    is_stopped = true;
    is_filling = false;
  }
  stop_lastButton = stop_currentButton;
  //Serial.println("stopped?");
}

void loop() {
  monitorFillButton();
}
