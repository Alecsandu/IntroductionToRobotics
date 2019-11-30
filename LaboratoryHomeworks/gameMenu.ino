#include <LiquidCrystal.h>
#include <string.h>
#include <EEPROM.h>

#define MAX_NAME_LEN 10

// variables for the LCD pins
const int RSPin = 12;
const int EPin = 11;
const int D4Pin = 5;
const int D5Pin = 4;
const int D6Pin = 3;
const int D7Pin = 2;
LiquidCrystal lcd(RSPin, EPin, D4Pin, D5Pin, D6Pin, D7Pin);

// variables for the menu and various places of the program
char bestPlayer[MAX_NAME_LEN + 1] = "Nothing";
char currentPlayer[MAX_NAME_LEN + 1] = "Playa";
unsigned long int lastDisplayMillis = 0;
int displayDelay = 200;
int startingLevelValue = 0;
int currentLives = 3;
unsigned long int startingTime = 0;
int menuSelected = 1;
int menuWasSelected = 0;
int funcChange = 1;
int gameHighScore = 0;
int charNo = 0;
int justPass = 0;

// variables for the joystick pins and different functions
const int pinX = A5;
const int pinY = A4;
const int buttonPin = 8;
const int lowAxis = 100;
const int highAxis = 800;
int xAxisReset = 1;
int lastButtonValue = 1;

void setup() {
  lcd.begin(16, 2);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
  //freeSpace();   // I call this function when I want to get rid of the highest score that is stored  
  loadHighscore();
}

void loop() {
  
  if (!menuWasSelected) {
    if (buttonGotPressed()){
      menuWasSelected = 1;
    }
    else {
      selectOption();
    }
  }
  else {
    if (menuSelected == 1) {
      playGame();    
    }
    else if (menuSelected == 2) {
      changeSettings();
    }
    else if (menuSelected == 3) {
      displayHighScore();
    }
  }
}

void saveHighscore() {
  int v[MAX_NAME_LEN + 1];
  for (int i = 0; i <= MAX_NAME_LEN; i++) {
    v[i] = (int)bestPlayer[i];
  }
  EEPROM[0] = gameHighScore;
  for (int i = 0; i <= MAX_NAME_LEN; i++) {
    EEPROM[i + 1] = v[i];
  }
}

void loadHighscore() {
  if (EEPROM[0] == 0) {
    return;
  }
  int v[MAX_NAME_LEN + 1];
  for (int i = 0; i <= MAX_NAME_LEN; i++) {
    v[i] = EEPROM[i + 1];
  }
  Serial.print("\n");
  gameHighScore = EEPROM[0];
  for (int i = 0; i <= MAX_NAME_LEN; i++) {
    bestPlayer[i] = (char)v[i];
    Serial.print(v[i]);
    Serial.print(" ");
    Serial.println(bestPlayer[i]);
  }
}

void freeSpace() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}

void displayGameStatus(int currentLives, int levelValue, int currentScore) {
  if (millis() - lastDisplayMillis > displayDelay) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Lives:");
    lcd.print(currentLives);
    lcd.print(" Level:");
    lcd.print(levelValue);
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.print(currentScore);
    lastDisplayMillis = millis();
  }
}

int gameEnded() {
  if (millis() - startingTime > 9900) {
    return 1;
  }
  return 0;
}

int buttonGotPressed() {
  int buttonValue = digitalRead(buttonPin);
  if (buttonValue == 0 && lastButtonValue == 1) {
    lastButtonValue = 0;
    return 1;
  }
  if (buttonValue == 1 && lastButtonValue == 0) {
      lastButtonValue = 1;
  }
  return 0;
}

void playGame() {
  if (funcChange) {
    currentLives = 3;
    startingTime = millis();
    funcChange = 0;
  }
  int levelValue = startingLevelValue + (millis() - startingTime)/5000;
  int currentScore = levelValue * 3;
  if (currentScore > gameHighScore) {
    gameHighScore = currentScore;
    strncpy(bestPlayer, currentPlayer, MAX_NAME_LEN + 1);
    saveHighscore();
  }
  displayGameStatus(currentLives, levelValue, currentScore);
  if (gameEnded()) {
    lcd.clear();
    lcd.print("Congratulations!");
    lcd.setCursor(0, 1);
    lcd.print("Press the button");
    while (1) {
      if (buttonGotPressed()) {
        menuWasSelected = 0;
        funcChange = 1;
        break;
      }
    }
  }
}

void refreshName() {
  while (Serial.available() > 0) {
    int incomingByte = Serial.read();
    if (justPass) {
      continue;
    }
    char ch = (char)incomingByte;
    if (incomingByte != 10) {
      currentPlayer[charNo] = ch;
      charNo++;
    }
    else {
      currentPlayer[charNo] = NULL;
      charNo = 0;
      justPass = 1;
    }
    if (charNo == MAX_NAME_LEN) {
      currentPlayer[charNo] = NULL;
      charNo = 0;
      justPass = 1;
    }
  }
  justPass = 0;
}

void changeSettings() {
  if (millis() - lastDisplayMillis > displayDelay) {
    lcd.clear();
    lcd.print("Level: ");
    lcd.print(startingLevelValue);
    lcd.setCursor(0, 1);
    lcd.print(currentPlayer);
    lastDisplayMillis = millis();
  }
  int xAxis = analogRead(pinX);
  if (xAxis < lowAxis && xAxisReset) {
    startingLevelValue--;
    if (startingLevelValue < 0) {
      startingLevelValue = 9;
    }
    xAxisReset = 0;
  }
  else if (xAxis > highAxis && xAxisReset) {
    startingLevelValue++;
    if (startingLevelValue > 9) {
      startingLevelValue = 0;
    }
    xAxisReset = 0;
  }
  else if (xAxis >= lowAxis && xAxis <= highAxis){
    xAxisReset = 1;
  }
  refreshName();
  if (buttonGotPressed()) {
    menuWasSelected = 0;
  }
}

void displayHighScore() {
  lcd.clear();
  lcd.print(bestPlayer);
  lcd.print("  ");
  lcd.print(gameHighScore);
  while (1) {
    if (buttonGotPressed()) {
      menuWasSelected = 0;
      break;
    }
  }
}

void displayMenu() {
  if(millis() - lastDisplayMillis > displayDelay) {
    lcd.clear();
    lcd.setCursor(0, 0);
    if (menuSelected == 1) {
      lcd.print(">");
    }
    else {
      lcd.print(" ");
    }
    lcd.print("Start ");
    if (menuSelected == 2) {
      lcd.print(">");
    }
    else {
      lcd.print(" ");
    }
    lcd.print("Setup");
    
    lcd.setCursor(0, 1);
    if (menuSelected == 3) {
      lcd.print(">");
    }
    else {
      lcd.print(" ");
    }
    lcd.print("HighScore ");
    
    lastDisplayMillis = millis();
  }
}

void selectOption() {
  int xAxis = analogRead(pinX);
  if (xAxis < lowAxis && xAxisReset) {
    menuSelected--;
    if (menuSelected < 1) {
      menuSelected = 3;
    }
    xAxisReset = 0;
  }
  else if (xAxis > highAxis && xAxisReset) {
    menuSelected++;
    if (menuSelected > 3) {
      menuSelected = 1;
    }
    xAxisReset = 0;
  }
  else if (xAxis >= lowAxis && xAxis <= highAxis){
    xAxisReset = 1;
  }
  displayMenu();
}
