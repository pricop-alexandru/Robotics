#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin definitions
const int rockerSwitchPin = 4; // Rocker switch connected to D4
const int pushButtonPin = 3;   // Pushbutton connected to D3
const int relayPin = 5;        // Relay control pin connected to D5

// Variables to track state
bool lightbulbState = false;
bool lcdState = true;
unsigned long runtime = 0;
unsigned long lastUpdateTime = 0;
unsigned long lastRockerSwitchTime = 0;
unsigned long lastPushButtonTime = 0;
const int debounceDelay = 200;

// Tracks the last stable state of the rocker switch
bool lastRockerSwitchState = LOW;

String formatTime(unsigned long milliseconds) {
  unsigned long seconds = milliseconds / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  seconds = seconds % 60;
  minutes = minutes % 60;

  // Use the String class to construct time formatting
  String formattedTime = "";
  if (hours < 10) formattedTime += "0";
  formattedTime += hours;
  formattedTime += ":";
  if (minutes < 10) formattedTime += "0";
  formattedTime += minutes;
  formattedTime += ":";
  if (seconds < 10) formattedTime += "0";
  formattedTime += seconds;

  return formattedTime;
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Lightbulb: ");
  lcd.print(lightbulbState ? "Off" : "On");

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(formatTime(runtime));
}

void setup() {
  // Initialize the LCD
  lcd.init(); // Specify dimensions of the LCD
  lcd.backlight();

  // Initialize pins
  pinMode(rockerSwitchPin, INPUT_PULLUP); // Use internal pull-up resistor
  pinMode(pushButtonPin, INPUT_PULLUP);  // Use internal pull-up resistor
  pinMode(relayPin, OUTPUT);

  // Initialize Serial for debugging
  Serial.begin(9600);
  // Initial display
  updateLCD();
}

void loop() {
  // Debouncing for the rocker switch
  bool currentRockerSwitchState = digitalRead(rockerSwitchPin);
  if (currentRockerSwitchState != lastRockerSwitchState) {
    lastRockerSwitchTime = millis(); // Reset debounce timer
  }

  if ((millis() - lastRockerSwitchTime > debounceDelay) &&
      currentRockerSwitchState != lightbulbState) {
    lightbulbState = !lightbulbState;
    digitalWrite(relayPin, lightbulbState ? HIGH : LOW); // Relay logic inverted due to pull-up
    runtime = 0; // Reset timer when lightbulb is turned on
    lastUpdateTime = millis(); // Reset last update time
    updateLCD(); // Update the LCD immediately when the lightbulb state changes
  }
  lastRockerSwitchState = currentRockerSwitchState;

  // Debouncing for the pushbutton
  if (digitalRead(pushButtonPin) == LOW &&
      (millis() - lastPushButtonTime > debounceDelay)) {
    lastPushButtonTime = millis();
    lcdState = !lcdState;
    if (lcdState) {
      lcd.noBacklight();
    } else {
      lcd.backlight();
    }
  }

  // Update runtime and LCD display if the lightbulb is on
  if (!lightbulbState) {
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime >= 1000) {
      runtime += 1000; // Increment runtime by 1 second
      lastUpdateTime = currentTime;
      updateLCD(); // Update LCD every second while lightbulb is on
    }
  } else {
    lastUpdateTime = millis(); // Reset last update time when lightbulb is off
  }
}
