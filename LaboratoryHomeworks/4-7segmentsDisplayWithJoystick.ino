// pins for the 4 digit 7-segment display
const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = 4;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;

// pins for the joystick
const int pinX = A0;
const int pinY = A1;
const int pinButton = A2; 

const int segSize = 8;
const int noOfDisplays = 4;
const int noOfDigits = 10;

// variables used for the joystick
int xAxis;
int yAxis;
const int highAxis = 750;
const int lowAxis = 200;
int buttonState;
int lastButtonState = HIGH;

int dpState = LOW;
int digitSelected = 0;
int blinkStatus = LOW;

const int millisWaitInterval = 200;
const int millisBlinkInterval = 200;
unsigned long lastMillis = 0;
unsigned long lastBlink = 0;

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

int digits[noOfDisplays] = {
  pinD1, pinD2, pinD3, pinD4
};   

int numbers[noOfDisplays] = {
  0, 0, 0, 0
};
 
byte digitMatrix[noOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void displayNumber(byte digit, byte decimalPoint) {
  for(int i = 0; i < segSize - 1; ++i) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  digitalWrite(segments[segSize - 1], decimalPoint);
}

void showDigit(int num) {
  for(int i = 0; i < noOfDisplays; ++i) {
    digitalWrite(digits[i], HIGH);
  }
  digitalWrite(digits[noOfDisplays - num - 1], LOW);
}

void displayNumbers(int nums[]) {
  for(int i = 0; i < noOfDisplays; ++i) {
    showDigit(i);
    if(digitSelected == i) {
      if((millis() - lastBlink) >= millisBlinkInterval) {
        blinkStatus = !blinkStatus;
        lastBlink = millis();
      }
      displayNumber(nums[i], blinkStatus);
    }
    else if(digitSelected == -i -1) {
      displayNumber(nums[i], HIGH);
    }
    else {
      displayNumber(nums[i], LOW);
    }
    // for stability
    delay(1);
  }
}

void setup() {
  pinMode(pinButton, INPUT_PULLUP);
  for(int i = 0; i < segSize; ++i) {
    pinMode(segments[i], OUTPUT);  
  }
  for(int i = 0; i < noOfDisplays; ++i) {
    pinMode(digits[i], OUTPUT);  
  }
}

void loop() {
  xAxis = analogRead(pinX);
  yAxis = analogRead(pinY);
  buttonState = digitalRead(pinButton);
  if(digitSelected >= 0) {
    // here the value of the digit selected is increased/ decreased here
    if((yAxis > highAxis) && (millis() - lastMillis > millisWaitInterval)) {
      numbers[digitSelected]--;
      if(numbers[digitSelected] < 0) {
        numbers[digitSelected] = 9;
      }
      lastMillis = millis();
    }
    if((yAxis < lowAxis) && (millis() - lastMillis > millisWaitInterval)) {
      numbers[digitSelected]++;
      if(numbers[digitSelected] == noOfDigits) {
        numbers[digitSelected] = 0;
      }
      lastMillis = millis();
    }
    if(buttonState != lastButtonState) {
      if(buttonState == LOW) {
        dpState = !dpState;
        digitSelected = -digitSelected - 1;
      }
      lastButtonState = buttonState;
    }
  } else {
    //     here we change the digit for which we will increase or decrease its value
    if((xAxis > highAxis) && (millis() - lastMillis > millisWaitInterval)) {
      digitSelected--;
      if(digitSelected == - noOfDisplays - 1) {
        digitSelected = -1;
      }
      lastMillis = millis();
    }
    if((xAxis < lowAxis) && (millis() - lastMillis > millisWaitInterval)) {
      digitSelected++;
      if(digitSelected == 0) {
        digitSelected = -noOfDisplays;
      }
      lastMillis = millis();
    }
    if(buttonState != lastButtonState) {
      if(buttonState == LOW) {
        dpState = !dpState;
        digitSelected = -digitSelected - 1;
      }
      lastButtonState = buttonState;
    }
  }
  displayNumbers(numbers);
}
