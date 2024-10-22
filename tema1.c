#include<Arduino.h>
#define GREEN_LED 5   // LED verde
#define RED_LED 6     // LED rosu
#define BLUE_LED 4    // LED albastru 
#define L1_LED 10     // pin pt led1 (25%)
#define L2_LED 9      // pin pt led2 (50%)
#define L3_LED 8      // pin pt led3 (75%)
#define L4_LED 7      // pin pt led4 (100%)
#define BTN1 2        // pin pt butonul de start
#define BTN2 3        // pin pt butonul de stop

// constante de timp
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
const unsigned long debounceDelay = 50;
const long blinkInterval = 500;
const long loaderInterval = 3000;

// butoanele sunt active pe LOW, asa ca punem starile initiale
int lastButton1State = HIGH;
int lastButton2State = HIGH;
bool charging = false;
bool stopRequested = false;
unsigned long previousMillis = 0;

void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(L1_LED, OUTPUT);
  pinMode(L2_LED, OUTPUT);
  pinMode(L3_LED, OUTPUT);
  pinMode(L4_LED, OUTPUT);

  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
}

void stopCharging() {
  stopRequested = true;
  charging = false;
	
  	// aratarea pasului final la oprirea fortata a incarcarii
  for (int i = 0; i < 3; i++) {
    digitalWrite(L1_LED, LOW);
    digitalWrite(L2_LED, LOW);
    digitalWrite(L3_LED, LOW);
    digitalWrite(L4_LED, LOW);
    delay(500);
    digitalWrite(L1_LED, HIGH);
    digitalWrite(L2_LED, HIGH);
    digitalWrite(L3_LED, HIGH);
    digitalWrite(L4_LED, HIGH);
    delay(500);
  }

  digitalWrite(L1_LED, LOW);
  digitalWrite(L2_LED, LOW);
  digitalWrite(L3_LED, LOW);
  digitalWrite(L4_LED, LOW);

  stopRequested = false;

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
}

void blinkLED(int ledPin) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= blinkInterval) {
    previousMillis = currentMillis;
    digitalWrite(ledPin, !digitalRead(ledPin));  // toggle pt starea led-ului
  }
}

void startCharging() {
  charging = true;
  stopRequested = false;
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);

  // secventa non-blocking cu comanda millis
  unsigned long stepStartTime = millis();

  for (int step = 1; step <= 4; step++) {
    while (millis() - stepStartTime < loaderInterval) {
      // una din multiplele verificari ale butonului 2 pentru a opri incarcarea
      if (!digitalRead(BTN2)) {
        stopCharging();
        return;
      }

      // clipirea ledului pentru pasul curent
      switch (step) {
        case 1: blinkLED(L1_LED); break;
        case 2: blinkLED(L2_LED); break;
        case 3: blinkLED(L3_LED); break;
        case 4: blinkLED(L4_LED); break;
      }
    }

    // dupa clipire pornim complet acel led
    switch (step) {
      case 1: digitalWrite(L1_LED, HIGH); break;
      case 2: digitalWrite(L2_LED, HIGH); break;
      case 3: digitalWrite(L3_LED, HIGH); break;
      case 4: digitalWrite(L4_LED, HIGH); break;
    }

    // urmatorul pas pt ledurile din case-uri
    stepStartTime = millis();
  }

  // finalul procesului de incarcare
  for (int i = 0; i < 3; i++) {
    if (!digitalRead(BTN2)) {
      stopCharging();
      return;
    }
    digitalWrite(L1_LED, LOW);
    digitalWrite(L2_LED, LOW);
    digitalWrite(L3_LED, LOW);
    digitalWrite(L4_LED, LOW);
    delay(500); // imi pare rau pentru delay
    digitalWrite(L1_LED, HIGH);
    digitalWrite(L2_LED, HIGH);
    digitalWrite(L3_LED, HIGH);
    digitalWrite(L4_LED, HIGH);
    delay(500);
  }

  // resetare stadiu dupa incarcare
  digitalWrite(L1_LED, LOW);
  digitalWrite(L2_LED, LOW);
  digitalWrite(L3_LED, LOW);
  digitalWrite(L4_LED, LOW);

  charging = false;
  stopRequested = false;

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
}

void loop() {
  // citire buton 1 cu debouncing
  int button1Reading = digitalRead(BTN1);
  if (button1Reading != lastButton1State) {
    lastDebounceTime1 = millis();
  }
  if ((millis() - lastDebounceTime1) > debounceDelay) {
    if (button1Reading == LOW && !charging && !stopRequested) {
      startCharging();
    }
  }
  lastButton1State = button1Reading;

  // citire buton 2 cu debouncing
  int button2Reading = digitalRead(BTN2);
  if (button2Reading != lastButton2State) {
    lastDebounceTime2 = millis();
  }
  if ((millis() - lastDebounceTime2) > debounceDelay) {
    if (button2Reading == LOW && (charging || stopRequested)) {
      stopCharging();
    }
  }
  lastButton2State = button2Reading;
}
