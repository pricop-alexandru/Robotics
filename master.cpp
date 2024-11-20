#include <Arduino.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <Wire.h>

// Initializam LCD-ul si servomotorul
LiquidCrystal lcd(8, 7, 6, 5, 4, 3);
Servo gameTimer;

// Definim pinii si valorile
const int startButton = A1;
const int startThreshold = 700;
const int servoControlPin = 9;
const int servoMaxDegrees = 180;
const unsigned long gameLength = 30000; // Durata jocului in ms (30 secunde)

// Culori disponibile pentru LED-uri
char ledColors[] = {'r', 'g', 'b'};
unsigned long startMillis = 0;
unsigned long lastRoundMillis = 0;
unsigned long roundDuration = 3000; // Durata rundei: 3 secunde
int totalRounds = gameLength / roundDuration;

// Variabile de stare pentru joc
bool isGameRunning = false;
bool awaitingFeedback = false;
char feedback;
String playerOne, playerTwo;
int playerOnePoints = 0;
int playerTwoPoints = 0;
int currentRound = 0;
bool playerOneTurn = true;

void setup() {
    Serial.begin(9600);
    SPI.begin();
    pinMode(SS, OUTPUT);
    digitalWrite(SS, HIGH);
    pinMode(startButton, INPUT);

    lcd.begin(16, 2);
    gameTimer.attach(servoControlPin);

    lcd.print("Apasa start!");
}
void updateScores(char scoreValue) {
    int score = 0;
    switch (scoreValue) {
        case 'a': score = 5; break;
        case 'b': score = 3; break;
        case 'c': score = 1; break;
        case 'i': score = 0; break;
    }

    if (!playerOneTurn) {
        playerOnePoints += score;
    } else {
        playerTwoPoints += score;
    }
}

char sendCommand(char command) {
    digitalWrite(SS, LOW);
    char response = SPI.transfer(command);
    digitalWrite(SS, HIGH);
    return response;
}

void displayWinner() {
    lcd.clear();
    lcd.print(playerOne + ": ");
    lcd.print(playerOnePoints);
    delay(2000);

    lcd.clear();
    lcd.print(playerTwo + ": ");
    lcd.print(playerTwoPoints);
    delay(2000);

    lcd.clear();
    if (playerOnePoints > playerTwoPoints) {
        lcd.print("Castigator: " + playerOne);
    } else if (playerTwoPoints > playerOnePoints) {
        lcd.print("Castigator: " + playerTwo);
    } else {
        lcd.print("Egalitate!");
    }
    delay(2000);
}

void resetGame() {
    playerOnePoints = 0;
    playerTwoPoints = 0;
    currentRound = 0;
    playerOneTurn = true;
    lcd.clear();
    lcd.print("Apasa start!");
}
void sendColorSignal() {
    char colorSignal = ledColors[random(0, 3)];
    sendCommand(colorSignal);
    Serial.print("Trimis: ");
    Serial.println(colorSignal);
}
void loop() {
    if (!isGameRunning && analogRead(startButton) > startThreshold) {
        delay(200);
        isGameRunning = true;

        // Cerem numele jucatorilor
        lcd.clear();
        lcd.print("Player 1:");
        while (Serial.available() == 0) {}
        playerOne = Serial.readStringUntil('\n');

        lcd.clear();
        lcd.print("Player 2:");
        while (Serial.available() == 0) {}
        playerTwo = Serial.readStringUntil('\n');

        startMillis = millis();
    }

    if (isGameRunning) {
        unsigned long currentMillis = millis();

        // Finalizarea jocului dupa 30 secunde
        if (currentMillis - startMillis >= gameLength) {
            isGameRunning = false;
            displayWinner();
            resetGame();
            return;
        }

        // Rotim servomotorul in functie de timpul scurs
        int currentAngle = map(currentMillis - startMillis, 0, gameLength, 0, servoMaxDegrees);
        gameTimer.write(currentAngle);

        // Gestionam runde la fiecare 3 secunde
        if (currentMillis - lastRoundMillis >= roundDuration) {
            if (currentRound < totalRounds) {
                lcd.clear();
                lcd.print((playerOneTurn ? playerOne : playerTwo) + " scor: ");
                lcd.print((playerOneTurn ? playerOnePoints : playerTwoPoints));
                sendColorSignal();

                delay(3000);
                awaitingFeedback = true;
                lastRoundMillis = currentMillis;
                playerOneTurn = !playerOneTurn;
                currentRound++;
            }
        }

        // Primim si procesam feedback-ul slave-ului
        if (awaitingFeedback) {
            feedback = sendCommand('#'); // Trimitem un comanda fictiva pentru a primi feedback
            updateScores(feedback);
            awaitingFeedback = false;
        }
    }
}


