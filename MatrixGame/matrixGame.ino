#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <LedControl.h>
#include <string.h>
#define MAX_NAME_LEN 10
/*----------------------------------For lcd----------------------------------*/
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
int currentLives = 3; // Deault amount of lifes  
unsigned long int startingTime = 0;
int menuSelected = 1;
int menuWasSelected = 0;
int funcChange = 1;
int gameHighScore = 0;
int charNo = 0;
int justPass = 0;
/*----------------------------------For matrix----------------------------------*/
const int brightness = 8;
const int dinPin = 12;
const int clkPin = 11;
const int loadPin = 10;
const int noOfDrivers = 1;
/*----------------------------------Level 1 map----------------------------------*/
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
/*----------------------------------Level 2 map----------------------------------*/
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
/*----------------------------------Level 3 map----------------------------------*/
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
bool helloMatrice[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {0, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};
const int timeToGetFood3 = 4000;
LedControl lc = LedControl(dinPin, clkPin, loadPin, noOfDrivers);// DIN, CLK, LOAD, NoOfDrivers

/*----------------------------------For joystick----------------------------------*/
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
/*----------------------------------For snako----------------------------------*/
int foodX = 2;
int foodY = 2;
int oldFoodX = 2;
int oldFoodY = 2;
bool hitFood = false;
bool initFood = false;
int scoreFood = 0;
int levelValue = 1;
int currentScore = 0;
bool eaten = false;
unsigned long previousMillis = 0;
const long interval = 200;
bool foodBlinked = true;
unsigned long prevMil = 0;
bool ok = false;
bool moreLife = false;
unsigned long theTime = 0;
unsigned long prevTime = 0;
unsigned long playTimeDelay = 0; // For the 25 delay

void setup() {
  randomSeed(analogRead(0));
  lcd.begin(16, 2);
  lc.shutdown(0, false); // Turn of power saving, enables display
  lc.setIntensity(0, brightness); // Set brightness (0~15 possible values)
  lc.clearDisplay(0);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  loadHighScore(); // Load the highest score when the game is turned on
  helloWorld(); // Print an welcome message when the game is turned on
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
    else if (menuSelected == 4) {
      info();
    }
  }
}

/*
  Displays a welcome message on the lcd when the game is turned on
*/
void helloWorld() {
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      if (helloMatrice[row][col] > 0) {
        lc.setLed(0, row, col, true);
      }
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   Agility Snake");
  lcd.setCursor(0, 1);
  lcd.print("Test your speed");
  delay(3000); // Here the delay is used because it will be executed only once
  lcd.clear();
  lc.clearDisplay(0);
}

/* 
  This function helps players to pick an option from the main menu (Start, Setup, Highscore or Info) with the help of the joystick
*/
void selectOption() {
  int xAxis = analogRead(pinX);
  if (xAxis < lowAxis && xAxisReset) {
    menuSelected--;
    if (menuSelected < 1) {
      menuSelected = 4;
    }
    xAxisReset = 0;
  }
  else if (xAxis > highAxis && xAxisReset) {
    menuSelected++;
    if (menuSelected > 4) {
      menuSelected = 1;
    }
    xAxisReset = 0;
  }
  else if (xAxis >= lowAxis && xAxis <= highAxis) {
    xAxisReset = 1;
  }
  displayMenu();
}

void displayMenu() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastDisplayMillis > displayDelay) {
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
    if (menuSelected == 4) {
      lcd.print(">");
    }
    else {
      lcd.print(" ");
    }
    lcd.print("Info");
    
    lastDisplayMillis = currentMillis;
  }
}

/*
  Before each new game some of the variables are going to be reinitialized
*/
void initGame(int lvl) {
  if (!initFood) {
    theTime = 30;
    currentScore = 0;
    posX = 4;
    posY = 4;
    oldPosX = 4;
    oldPosY = 4;
    if (lvl == 1 ) {
      foodX = random(0, 8);
      foodY = random(0, 8);
      while (checkIfOneIsOnPos(lvl, foodX, foodY) || (foodX == posX && foodY == posY) || (foodX == oldFoodX && foodY == oldFoodY)) {
        foodX = random(0, 8);
        foodY = random(0, 8);
      }
    }
    else {
      foodX = random(1, 7);
      foodY = random(1, 7);
      while (checkIfOneIsOnPos(lvl, foodX, foodY) || (foodX == posX && foodY == posY) || (foodX == oldFoodX && foodY == oldFoodY)) {
        foodX = random(1, 7);
        foodY = random(1, 7);
      }
    }
    lc.clearDisplay(0);
    oldFoodX = foodX;
    oldFoodY = foodY;
    lc.setLed(0, foodX, foodY, true);
    initFood = true;
  }
}

void playGame(int level) {
  // Check if its the first time that this function is called when a new game is started
  if (funcChange) {
    currentLives = 3;
    previousMillis = 0;
    startingTime = millis();
    funcChange = 0;
    currentScore = 0;
    levelValue = level * random(1, 11);
    loadMatrixLevel(level);
  }
  if(moveLed(level) && eaten == true) {
    currentScore += levelValue;
    makeNewFood(level);
    eaten = false;
  }
  unsigned long st = millis();
  
  // Each second decreases the amount of time that is left (only the variable that is used to display it)
  if (st - prevTime >= 1000) {
    prevTime = st;
    --theTime;
  }
  unsigned long start = millis();
  // Check if the player arrived in time at the food spot and if not I generate a new position for the food spot
  if(start - prevMil >= getInterval(level)) {
    prevMil = start;
    if(ok){
      makeNewFood(level);
      ok = !ok;
    } else {
      ok = !ok;
    }
  }
  blinkFood();
  // If the highscore was bested, award the player with a bonus life
  if (currentScore > gameHighScore) {
    if (!moreLife) {
      currentLives++;
      moreLife = true;
    }
    gameHighScore = currentScore;
    strncpy(bestPlayer, currentPlayer, MAX_NAME_LEN + 1);
    saveHighScore();
  }
  displayGameStatus(level);
  if (gameEnded()) {
    /*
     * Check if the player died or the time is up 
     * If the player died load a matrix which displays an X on the 8x8 matrix and show the score on the lcd
     * Otherwise leave the matrix unchanged and display the score on the lcd
     */
    if (currentLives != 0) {
      lcd.clear();
      lcd.print("Your score:");
      lcd.print(currentScore);
      lcd.setCursor(0, 1);
      lcd.print("Press THE button");
    }
    else {
      lcd.clear();
      lc.clearDisplay(0);
      lcd.setCursor(4, 0);
      lcd.print("YOU DIED :(");
      lcd.setCursor(0, 1);
      lcd.print("Your score:");
      lcd.print(currentScore);
      for (int row = 0;row < 8;row++) {
        for(int col = 0;col < 8;col++) {
          if (gameOverMatrix[row][col]>0) {
            lc.setLed(0, row, col, true);
          }
        }
      }
    }
    /*
      The statement 'while (1)' is used because the program just waits for the user to push the button to go back to the main menu,
      also nothing else is waiting to be done apart from the push of the button and no other functions are called
    */
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

/*
  Here the player can change the map/level,
  or chnage the name if the serial monitor is open,
  or can reset the highscore (this is not shown in anyway on the lcd,
      the player has to know that by using the yAxis of the joystick and
      turning it on the right side the score will reset)
*/
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
  int yAxis = analogRead(pinY);
  Serial.println(yAxis);
  
  if (yAxis < 100) {
    freeSpace();
  }
  
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

void displayHighScore() {
  // If there is no score saved display 0
  if (EEPROM[0] == 0) {
    lcd.clear();
    lcd.print("Nothing 0");
  } else {
    lcd.clear();
    lcd.print(bestPlayer);
    lcd.print(" ");
    lcd.print(gameHighScore);
  }
 
  /*
    The statement 'while (1)' is used because the program doesn't stop something from executing
    and just waits for the player to press the button after it choosed what it wants to do
  */
  while (1) {
    if (buttonGotPressed()) {
      menuWasSelected = 0;
      break;
    }
  }
}

/*
  This function displays the name of the game, creator, my GitHub link and @UnibucRobotics
  The 'delay' function is used here because 'info()' is executed only once and after that the program goes back at displaying the main menu on the lcd
*/
void info() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game: Snake");
    lcd.setCursor(0, 1);
    lcd.print("DoneBy:Alexandru");

    delay(2000);

    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("GitHub link: ");

    delay(2000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("https://github.c");
    lcd.setCursor(0, 1);
    lcd.print("om/alecsandu");

    delay(2000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("@ UnibucRobotics");

    delay(2000);
    menuWasSelected = 0; // This signals that I don't want to display 'info()' again on the lcd 
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

/*
  This function is used to reset the highscore
*/
void freeSpace() {
  gameHighScore = 0;
  bestPlayer[0] = "Nothing";
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}

/*
  Utility function which helps us determine if the joystick button was pressed or not
*/
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

/*
  After the game starts this function displays on the lcd:
  - number of lives
  - level
  - score
  - remaining time until the game ends
*/
void displayGameStatus(int lvl) {
  unsigned long currentMillis = millis();
  if (currentMillis - lastDisplayMillis > displayDelay) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Lives:");
    lcd.print(currentLives);
    lcd.print(" Level:");
    lcd.print(lvl);
    lcd.setCursor(0, 1);
    lcd.print("Score:");
    lcd.print(currentScore);
    lcd.print(" Time:");
    lcd.print(theTime);
    lastDisplayMillis = currentMillis;
  }
}

/*
  Loads a level depending on the matrix that coresponds to the selected level
*/
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

/*
  If the time is up or if the player lost all its lives the game stops
*/
int gameEnded() {
  unsigned long mil = millis();
  if (mil - startingTime > 30000) { 
    initFood = false;
    return 1;
  }
  else if (currentLives == 0) {
    initFood = false;
    return 1;
  }
  return 0;
}

/*
  This function generates a new food position that is NOT in the walls or on the current position of the player or on the same position of the old food once again
*/
void makeNewFood(int lvl) {
  oldFoodX = foodX;
  oldFoodY = foodY;
  lc.setLed(0, oldFoodX, oldFoodY, false);
  if (lvl == 1) {
    foodX = random(0, 8);
    foodY = random(0, 8);
    while ((checkIfOneIsOnPos(lvl, foodX, foodY)==true) || (foodX == posX && foodY == posY) || foodX < 1 || foodX > 6 || foodY < 1 || foodY > 6 || (foodX == oldFoodX && foodY == oldFoodY)) {
      foodX = random(0, 8);
      foodY = random(0, 8);
    }
  }
  else {
    foodX = random(1, 7);
    foodY = random(1, 7);
    while ((checkIfOneIsOnPos(lvl, foodX, foodY)==true) || (foodX == posX && foodY == posY) || foodX < 1 || foodX > 6 || foodY < 1 || foodY > 6 || (foodX == oldFoodX && foodY == oldFoodY)) {
      foodX = random(1, 7);
      foodY = random(1, 7);
    }
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

/*
  This function returns the amount of time that a player has to get to a food spot until a new one is generated
*/
int getInterval(int lvl) {
  if (lvl == 1)
    return timeToGetFood1;
  else if(lvl == 2)
    return timeToGetFood2;
  else if(lvl == 3)
    return timeToGetFood3;
}

/*
  This function checks if there is a wall on the position (x, y) from the map of the level 'level'
*/
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

/*
  This function changes the position of the player depending on his choice up, down, left or right
  if in the way is a wall the number of lives will decrease,
  at the end, this function checks if the player arrived at the food spot and returns true or false depending on that
*/
bool moveLed(int level) {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  unsigned long currentTime = millis();
  if (currentTime - playTimeDelay >= 25) {
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
      if (checkIfOneIsOnPos(level, posX, posY - 1)) {
        currentLives--;
      }
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
      if (checkIfOneIsOnPos(level, posX, posY + 1)) {
        currentLives--;
      }
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
    playTimeDelay = currentTime;
    if (checking && eaten == false) {
      eaten = true;
      return true;
    }
    return false;
  }
}

/*
  If the player wants to change the name, it has to use the serial monitor and put it as an input
*/
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
