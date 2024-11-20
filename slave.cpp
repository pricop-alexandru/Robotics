#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
// Pinii pentru butoane si LED-uri
const int p1ButtonPin = A1;
const int p2ButtonPin = A0;
const int p1RedLed = 5;
const int p1GreenLed = 6;
const int p1BlueLed = 7;
const int p2RedLed = 2;
const int p2GreenLed = 3;
const int p2BlueLed = 4;

volatile char currentCommand = '\0';
volatile bool hasNewCommand = false;

unsigned long buttonPressStart = 0;
bool waitingForButton = true;
bool player1Active = true;

// Timpuri de raspuns
int fastTime = 900;
int mediumTime = 1800;
int slowTime = 3000;

void setup() {
    Serial.begin(9600);
    SPCR |= bit(SPE); // SPI slave
    pinMode(MISO, OUTPUT); // MISO ca output
    SPI.attachInterrupt();

    pinMode(p1ButtonPin, INPUT);
    pinMode(p2ButtonPin, INPUT);

    // Setam LED-urile ca output
    pinMode(p1RedLed, OUTPUT);
    pinMode(p1GreenLed, OUTPUT);
    pinMode(p1BlueLed, OUTPUT);
    pinMode(p2RedLed, OUTPUT);
    pinMode(p2GreenLed, OUTPUT);
    pinMode(p2BlueLed, OUTPUT);
}

ISR(SPI_STC_vect) {
    char receivedChar = SPDR;
    if (receivedChar != '#') {
        currentCommand = receivedChar;
        hasNewCommand = true;
    }
}
void checkButtonPress() {
    unsigned long elapsed = millis() - buttonPressStart;
    int playerButton = player1Active ? analogRead(p1ButtonPin) : analogRead(p2ButtonPin);

    if (playerButton > 400) {
        char response;
        if (elapsed <= fastTime) {
            response = 'a';
        } else if (elapsed <= mediumTime) {
            response = 'b';
        } else if (elapsed <= slowTime) {
            response = 'c';
        } else {
            response = 'i';
        }

        SPDR = response;
        player1Active = !player1Active;
        waitingForButton = false;
    }
}

void resetLEDs() {
    digitalWrite(p1RedLed, LOW);
    digitalWrite(p1GreenLed, LOW);
    digitalWrite(p1BlueLed, LOW);
    digitalWrite(p2RedLed, LOW);
    digitalWrite(p2GreenLed, LOW);
    digitalWrite(p2BlueLed, LOW);
}
void activateLED(char color) {
    resetLEDs();
    int targetLed = (color == 'r') ? (player1Active ? p1RedLed : p2RedLed) :
                    (color == 'g') ? (player1Active ? p1GreenLed : p2GreenLed) :
                                     (player1Active ? p1BlueLed : p2BlueLed);
    digitalWrite(targetLed, HIGH);
}

void loop() {
    if (hasNewCommand) {
        hasNewCommand = false;

        if (currentCommand == 'r' || currentCommand == 'g' || currentCommand == 'b') {
            activateLED(currentCommand);
            buttonPressStart = millis();
            waitingForButton = true;
        } else {
            SPDR = '$';
        }
    }

    if (waitingForButton) {
        checkButtonPress();
    }
}
