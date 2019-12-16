#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <LedControl.h>
#include <string.h>
#define MAX_NAME_LEN 10
/*--------------------For lcd-----------------------------------*/
const int RSPin = 9;
const int EPin = 6;
const int D4Pin = 5;
const int D5Pin = 4;
const int D6Pin = 3;
const int D7Pin = 2;
LiquidCrystal lcd(RSPin, EPin, D4Pin, D5Pin, D6Pin, D7Pin);
char bestPlayer[MAX_NAME_LEN + 1] = "Nothing";
char currentPlayer[MAX_NAME_LEN + 1] = "Playa";
unsigned long int lastDisplayMillis = 0;
int displayDelay = 200;
int startingLevelValue = 1;
int currentLives = 3;             /*--------lives------*/  
unsigned long int startingTime = 0;
int menuSelected = 1;
int menuWasSelected = 0;
int funcChange = 1;
int gameHighScore = 0;
int charNo = 0;
int justPass = 0;
/*------------------For matrix----------------------------------*/
const int brightness = 8;
const int dinPin = 12;
const int clkPin = 11;
const int loadPin = 10;
const int noOfDrivers = 1;
//level 1
byte lvl1Matrix[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};
const int timeToGetFood1 = 6000;
//level 2
byte lvl2Matrix[8][8] = {
  {1, 0, 0, 0, 0, 0, 0, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 1}
};
const int timeToGetFood2 = 5000;
//level 3
byte lvl3Matrix[8][8] = {
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1}
};
bool gameOverMatrix[8][8] = {
  {1, 0, 0, 0, 0, 0, 0, 1},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {1, 0, 0, 0, 0, 0, 0, 1}
};
const int timeToGetFood3 = 4000;
LedControl lc = LedControl(dinPin, clkPin, loadPin, noOfDrivers);// DIN, CLK, LOAD, NoOfDrivers

/*----------------------For joystick------------------------------*/
const int pinX = A3;
const int pinY = A4;
const int buttonPin = 8;
int xValue = 0;
int yValue = 0;
bool movedX = false;
bool movedY = false;
int maxThreshold = 600;
int minThreshold = 400;
int oldPosX = 4;
int oldPosY = 4;
int posX = 4;
int posY = 4;
const int lowAxis = 100;
const int highAxis = 800;
int xAxisReset = 1;
int lastButtonValue = 1;
/*---------------------------For snek----------------------------------------*/
int foodX;
int foodY;
int oldFoodX;
int oldFoodY;
bool hitFood = false;
bool initFood = false;
int scoreFood = 0;
int levelValue = 1;
int currentScore = 0;
bool eaten = false;
unsigned long previousMillis = 0;
const long interval = 200;
bool foodBlinked = true;

void setup() {
  randomSeed(analogRead(0));
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lcd.begin(16, 2);
  lc.shutdown(0, false); // turn of power saving, enables display
  lc.setIntensity(0, brightness);// sets brightness (0~15 possible values)
  lc.clearDisplay(0);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  loadHighScore();
  //freeSpace();
  Serial.begin(9600);
}

void loop() {
  if (!menuWasSelected) {
    if (buttonGotPressed()) {
      menuWasSelected = 1;
    }
    else {
      selectOption();
    }
  }
  else {
    if (menuSelected == 1) {
      initGame(startingLevelValue);
      playGame(startingLevelValue);
    }
    else if (menuSelected == 2) {
      changeSettings();
    }
    else if (menuSelected == 3) {
      displayHighScore();
    }
  }
}

void initGame(int lvl) {
  if (!initFood) {
    currentScore = 0;
    posX = 4;
    posY = 4;
    oldPosX = 4;
    oldPosY = 4;
    foodX = random(0, 8);
    foodY = random(0, 8);
    while (checkIfOneIsOnPos(lvl, foodX, foodY) && foodX == posX && foodY == posY) {
      foodX = random(0, 8);
      foodY = random(0, 8);
    }
    lc.clearDisplay(0);
    oldFoodX = foodX;
    oldFoodY = foodY;
    lc.setLed(0, foodX, foodY, true);
    initFood = true;
  }
}

void saveHighScore() {
  int v[MAX_NAME_LEN + 1];
  for (int i = 0; i <= MAX_NAME_LEN; ++i) {
    v[i] = (int)bestPlayer[i];
  }
  EEPROM[0] = gameHighScore;
  for(int i = 0; i <= MAX_NAME_LEN; ++i) {
    EEPROM[i + 1] = v[i];
  }
}

void loadHighScore() {
  if (EEPROM[0] == 0) {
    return;
  }
  int v[MAX_NAME_LEN + 1];
  for (int i = 0; i <= MAX_NAME_LEN; ++i) {
    v[i] = EEPROM[i + 1];
  }
  Serial.print("\n");
  gameHighScore = EEPROM[0];
  for (int i = 0; i <= MAX_NAME_LEN; ++i) {
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

void displayGameStatus() {
  if (millis() - lastDisplayMillis > displayDelay) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Lives:");
    lcd.print(currentLives);
    lcd.print(" Level:");
    lcd.print(levelValue);
    lcd.setCursor(0, 1);
    lcd.print("Score:");
    lcd.print(currentScore);
    lastDisplayMillis = millis();
  }
}

void loadMatrixLevel(int level) {
  if (level == 1) {
    for (int row = 0;row < 8;row++) {
      for(int col = 0;col < 8;col++) {
         if (lvl1Matrix[row][col]>0) {
           lc.setLed(0, row, col, true);
         }
      }
    }
  }
  else if (level == 2) {
    for (int row = 0;row < 8;row++) {
      for(int col = 0;col < 8;col++) {
         if (lvl2Matrix[row][col]>0) {
           lc.setLed(0, row, col, true);
         }
      }
    }
  }
  else if (level == 3) {
    for (int row = 0;row < 8;row++) {
      for(int col = 0;col < 8;col++) {
         if (lvl3Matrix[row][col]>0) {
           lc.setLed(0, row, col, true);
         }
      }
    }
  }
}

int gameEnded() {
  if (millis() - startingTime > 30000) {
    initFood = false;
    return 1;
  }
  else if (currentLives == 0) {
    initFood = false;
    return 1;
  }
  return 0;
}

void makeNewFood(int lvl) {
  oldFoodX = foodX;
  oldFoodY = foodY;
  lc.setLed(0, oldFoodX, oldFoodY, false);
  foodX = random(0, 8);
  foodY = random(0, 8);
  while (checkIfOneIsOnPos(lvl, foodX, foodY) && foodX == posX && foodY == posY
  && foodX >= 1 && foodX <= 6 && foodY >= 1 && foodY <=6) {
    foodX = random(0, 8);
    foodY = random(0, 8);
  }
  lc.setLed(0, foodX, foodY, true);
}

bool checkIfHitFood() {
  if (posX == foodX && posY == foodY)
    return true;
  return false;
}

void blinkFood() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (foodBlinked) {
     lc.setLed(0, foodX, foodY, false);
     foodBlinked = false;
    }
    else {
      lc.setLed(0, foodX, foodY, true); 
      foodBlinked = true;
    }
  }
}

void playGame(int level) {
  if (funcChange) {
    currentLives = 3;
    previousMillis = 0;
    startingTime = millis();
    funcChange = 0;
    levelValue = level * random(1, 11);
    loadMatrixLevel(level);
  }
  if(moveLed(level) && eaten == true) {
    currentScore += levelValue;
    makeNewFood(level);
    eaten = false;
  }
  blinkFood();
  if (currentScore > gameHighScore) {
    gameHighScore = currentScore;
    strncpy(bestPlayer, currentPlayer, MAX_NAME_LEN + 1);
    saveHighScore();
  }
  displayGameStatus();
  if (gameEnded()) {
    if (currentLives != 0) {
      lcd.clear();
      lcd.print("Congratulations!");
      lcd.setCursor(0, 1);
      lcd.print("Press THE button");
    }
    else {
      lcd.clear();
      lc.clearDisplay(0);
      lcd.setCursor(4, 0);
      lcd.print("YOU DIED");
      for (int row = 0;row < 8;row++) {
        for(int col = 0;col < 8;col++) {
          if (gameOverMatrix[row][col]>0) {
            lc.setLed(0, row, col, true);
          }
        }
      }
    }
    while (1) {
      if (buttonGotPressed()) {
        menuWasSelected = 0;
        funcChange = 1;
        lc.clearDisplay(0);
        break;
      }
    }
  }
}

bool checkIfOneIsOnPos(int level, int x, int y) {
  if (level == 1) {
    if (lvl1Matrix[x][y] == 1) {
      return true;
    } else {
      return false;
    }
  }
  else if (level == 2) {
    if (lvl2Matrix[x][y] == 1) {
      return true;
    } else {
      return false;
    }
  }
  else if (level == 3) {
    if (lvl3Matrix[x][y] == 1) {
      return true;
    } else {
      return false;
    }
  }
}

bool moveLed(int level) {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  if (xValue > maxThreshold && movedX == false) {
    if (checkIfOneIsOnPos(level, posX + 1, posY)) {
      currentLives--;
    }
    if (posX < 8 && (checkIfOneIsOnPos(level, posX + 1, posY) == false)) {
      oldPosX = posX;
      oldPosY = posY;
      posX++;
    }
    else if (posX == 8 && (checkIfOneIsOnPos(level, 0, posY) == false)) {
      oldPosX = posX;
      oldPosY = posY;
      posX = 0;
    }
    movedX = true;
  }
  if (xValue < minThreshold && movedX == false) {
    if (checkIfOneIsOnPos(level, posX - 1, posY)) {
      currentLives--;
    }
    if (posX > 0 && (checkIfOneIsOnPos(level, posX - 1, posY) == false)) {
      oldPosX = posX;
      oldPosY = posY;
      posX--;
    }
    else if (posX == 0 && (checkIfOneIsOnPos(level, 7, posY) == false)) {
      oldPosX = posX;
      oldPosY = posY;
      posX = 7;
    }
    movedX = true;
  }
  if (xValue >= minThreshold && xValue <= maxThreshold) {
    movedX = false;
  }
  if (yValue > maxThreshold && movedY == false) {
    if (posY > 0 && (checkIfOneIsOnPos(level, posX, posY - 1) == false)) {
      oldPosY = posY;
      oldPosX = posX;
      posY--;
    }
    else if (posY == 0 && (checkIfOneIsOnPos(level, posX, 7) == false)) {
      oldPosY = posY;
      oldPosX = posX;
      posY = 7;
    }
    movedY = true;
  }
  if (yValue < minThreshold && movedY == false) {
    if (posY < 8 && (checkIfOneIsOnPos(level, posX, posY + 1) == false)) {
      oldPosY = posY;
      oldPosX = posX;
      posY++;
    }
    else if (posY == 8 && (checkIfOneIsOnPos(level, posX, 0) == false)) {
      oldPosY = posY;
      oldPosX = posX;
      posY = 0;
    }
    movedY = true;
  }
  if (yValue >= minThreshold && yValue <= maxThreshold) {
    movedY = false;
  }
  bool checking = checkIfHitFood();
  lc.setLed(0, oldPosX, oldPosY, false);
  lc.setLed(0, posX, posY, true);
  delay(25);
  if (checking && eaten == false) {
    eaten = true;
    return true;
  }
  return false;
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
  else if (xAxis >= lowAxis && xAxis <= highAxis) {
    xAxisReset = 1;
  }
  displayMenu();
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
    if (startingLevelValue < 1) {
      startingLevelValue = 3;
    }
    xAxisReset = 0;
  }
  else if (xAxis > highAxis && xAxisReset) {
    startingLevelValue++;
    if (startingLevelValue > 3) {
      startingLevelValue = 1;
    }
    xAxisReset = 0;
  }
  else if (xAxis >= lowAxis && xAxis <= highAxis) {
    xAxisReset = 1;
  }
  refreshName();
  if (buttonGotPressed()) {
    menuWasSelected = 0;
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

void displayMenu() {
  if (millis() - lastDisplayMillis > displayDelay) {
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

void displayHighScore() {
  lcd.clear();
  lcd.print(bestPlayer);
  lcd.print(" ");
  lcd.print(gameHighScore);
  while(1) {
    if (buttonGotPressed()) {
      menuWasSelected = 0;
      break;
    }
  }
}
