#include <Arduino.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <HardwareSerial.h>
#include <WString.h>

// Pinuri pentru LED RGB
const int ledRosuPin = 6;
const int ledVerdePin = 5;
const int ledAlbastruPin = 4;
const int ledPinuri[] = { ledRosuPin, ledVerdePin, ledAlbastruPin };

// Pinuri pentru Butoane
const int btnStartStopPin = 3;
const int btnCiclareDificultatePin = 2;
const int btnPinuri[] = { btnStartStopPin, btnCiclareDificultatePin };

// Debouncing pentru Butoane
volatile unsigned long timpUltimDebounceDif = 0;
const unsigned long timpDebounceDif = 50;

volatile unsigned long timpUltimDebounceStart = 0;
const unsigned long timpDebounceStart = 50;

// Constante de joc
const int timpRundaMs = 30000; // 30 secunde timp runda
const int durataAnimStart = 3; // Animatie start (3, 2, 1 si clipire LED)

// Numarul de cuvinte
const int numarCuvinte = 50;
const String cuvinte[numarCuvinte] = {
  "apple", "banana", "cat", "dog", "elephant",
  "fox", "grape", "house", "island", "jungle",
  "kangaroo", "lemon", "mountain", "night", "ocean",
  "pencil", "queen", "river", "sun", "tiger",
  "umbrella", "violin", "water", "xylophone", "yellow",
  "zebra", "airplane", "book", "carrot", "dance",
  "engine", "forest", "garden", "hat", "insect",
  "jacket", "kite", "lamp", "moon", "nest",
  "orange", "pizza", "quiet", "rain", "snow",
  "train", "unicorn", "violet", "whale", "yawn"
};

// Niveluri de dificultate
enum Dificultate {
  NECUNOSCUT = -1,
  USOR = 0,
  MEDIU = 1,
  GREU = 2
};

// Stare de joc
volatile bool ciclareDificultate = false;
volatile bool startJoc = false;
volatile bool stopJoc = false;
volatile bool jocActiv = false;
volatile bool schimbareCuvant = false;

enum Dificultate dificultate = Dificultate::NECUNOSCUT;
int timpPerCuvant = 0; // Timp maxim per cuvant, in functie de dificultate

long timpStartRundaMs = 0; // Timpul de start al rundei
int scor = 0; // Scorul
String cuvantCurent = ""; // Cuvantul curent
String textUtilizator = ""; // Textul introdus de utilizator

// Seteaza LED RGB
void setareLedRgb(int rosu, int verde, int albastru) {
  digitalWrite(ledRosuPin, rosu);
  digitalWrite(ledVerdePin, verde);
  digitalWrite(ledAlbastruPin, albastru);
}

// Handler pentru butonul de ciclare dificultate, cu debounce
void triggerCiclareDificultate() {
  if (millis() - timpUltimDebounceDif > timpDebounceDif) {
    timpUltimDebounceDif = millis();

    if (jocActiv) {
      return;
    }

    ciclareDificultate = true;
  }
}

// Functie pentru ciclare dificultate
void ciclareDificultateFunc() {
  ciclareDificultate = false;

  dificultate = (Dificultate)(((int)dificultate + 1) % 3); // Ciclare prin Usor, Mediu, Greu

  switch (dificultate) {
    case Dificultate::USOR:
      Serial.println("Mod usor activat!");
      timpPerCuvant = 4;
      break;
    case Dificultate::MEDIU:
      Serial.println("Mod mediu activat!");
      timpPerCuvant = 3;
      break;
    case Dificultate::GREU:
      Serial.println("Mod greu activat!");
      timpPerCuvant = 2;
      break;
    default:
      Serial.println("Dificultate invalida. Incearca din nou!");
      dificultate = Dificultate::NECUNOSCUT;
      timpPerCuvant = 0;
      break;
  }
}

// Handler pentru butonul Start/Stop, cu debounce
void triggerStartStop() {
  if (millis() - timpUltimDebounceStart > timpDebounceStart) {
    timpUltimDebounceStart = millis();

    if (jocActiv) {
      stopJoc = true;
    }
    else {
      startJoc = true;
    }
  }
}

// Functie de pornire a jocului
void startJocFunc() {
  startJoc = false;

  if (dificultate == Dificultate::NECUNOSCUT) {
    Serial.println("Te rog selecteaza dificultatea.");
    return;
  }

  scor = 0;
  textUtilizator = "";

  // Animatie de start
  for (int i = durataAnimStart; i > 0; i--) {
    Serial.println(i);
    setareLedRgb(HIGH, HIGH, HIGH);
    delay(500);
    setareLedRgb(LOW, LOW, LOW);
    delay(500);
  }

  setareLedRgb(HIGH, LOW, HIGH);

  noInterrupts();

  // Configurare Timer 1 pentru ceas de 12 MHz
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  const unsigned long ceas = 12000000; // Frecventa ceas de 12 MHz
  const unsigned long prescaler = 1024;
  const unsigned long tintaCompare = (ceas / prescaler) * timpPerCuvant - 1;

  OCR1A = tintaCompare;

  TCCR1B |= (1 << WGM12); // Activeaza modul CTC
  TCCR1B |= (1 << CS12) | (1 << CS10); // Seteaza prescaler la 1024
  TIMSK1 |= (1 << OCIE1A); // Activeaza intreruperea pentru compare

  interrupts();

  timpStartRundaMs = millis();
  jocActiv = true;
  schimbareCuvant = true;
}

// ISR pentru timer, pentru schimbarea cuvantului
ISR(TIMER1_COMPA_vect) {
  schimbareCuvant = true;
}

// Functie de schimbare a cuvantului
void schimbareCuvantFunc() {
  schimbareCuvant = false;

  TCNT1 = 0; // Reseteaza timerul

  int index = random(0, numarCuvinte);
  cuvantCurent = cuvinte[index];

  textUtilizator = ""; // Reseteaza textul utilizatorului

  Serial.print("\n\n");
  Serial.println(cuvantCurent);
}

// Reseteaza starea jocului
void resetareStareJoc() {
  setareLedRgb(HIGH, HIGH, HIGH);
  dificultate = Dificultate::NECUNOSCUT;
  timpPerCuvant = 0;
  ciclareDificultate = false;
  startJoc = false;
  stopJoc = false;
  jocActiv = false;
  scor = 0;
  schimbareCuvant = false;
  cuvantCurent = "";
  textUtilizator = "";

  noInterrupts();

  // Resetare Timer 1
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  OCR1A = 0;

  TCCR1B &= ~(1 << WGM12); // Dezactiveaza modul CTC
  TCCR1B &= ~(1 << CS12) & ~(1 << CS10); // Reseteaza prescalerul
  TIMSK1 &= ~(1 << OCIE1A); // Dezactiveaza intreruperea timerului
  // Am citit toata materia, o sa folosesc toata materia
  // registrii de avr pentru timere nu fac nimic ca am comenzi arduino de interrupturi (dar arata bine)
  interrupts();

  Serial.println("Selecteaza dificultatea inainte de a porni.");
}

// Functie de oprire a jocului
void oprireJoc() {
  Serial.print("\n\nScor: ");
  Serial.println(scor);

  resetareStareJoc();
}

// Initializare
void setup() {
  Serial.begin(28800);

  randomSeed(analogRead(0)); // Seteaza seed pentru secventa aleatoare

  for (int i = 0; i < 3; i++) {
    pinMode(ledPinuri[i], OUTPUT);
  }

  for (int i = 0; i < 2; i++) {
    pinMode(btnPinuri[i], INPUT_PULLUP);
  }

  attachInterrupt(digitalPinToInterrupt(btnCiclareDificultatePin), triggerCiclareDificultate, LOW);
  attachInterrupt(digitalPinToInterrupt(btnStartStopPin), triggerStartStop, LOW);

  resetareStareJoc();
}

// Bucla principala de operare
void loop() {
  if (ciclareDificultate) {
    ciclareDificultateFunc();
  }

  if (startJoc) {
    startJocFunc();
  }

  if (stopJoc) {
    oprireJoc();
  }

  if (jocActiv) {
    if (schimbareCuvant) {
      schimbareCuvantFunc();
    }

    if ((millis() - timpStartRundaMs >= timpRundaMs) && !stopJoc) {
      stopJoc = true;
    }

    if (Serial.available()) {
      char c = Serial.read(); 

      if (c == '\b') { 
        if (textUtilizator.length() > 0) {
          textUtilizator.remove(textUtilizator.length() - 1); 

          String clearText = "\r";
          for (unsigned int i = 0; i <= textUtilizator.length(); i++) {
            clearText += " ";
          }
          clearText += "\r";

          Serial.print(clearText);
          Serial.print(textUtilizator);
        }
      }
      else if (c == '\n') { 
        textUtilizator = ""; 
      }
      else { 
        textUtilizator += c; 
      }

      if (cuvantCurent.startsWith(textUtilizator)) { 
        setareLedRgb(LOW, HIGH, LOW); 
      }
      else { 
        setareLedRgb(HIGH, LOW, LOW); 
      }

      if (cuvantCurent.equals(textUtilizator)) { 
        scor++; 
        schimbareCuvant = true; 
      }
    }
  }
}
