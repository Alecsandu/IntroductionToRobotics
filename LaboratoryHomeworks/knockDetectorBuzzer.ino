//    Variables used by the passive buzzer
const int passiveBuzzerPin = A4;
int speakerValue = 0;
const int threshold = 20;
int knockValue = 0;

//  Used to wait 5 seconds before the tone starts   
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
const long intervalForBuzz = 5000;

//   Variables used for the active buzzer
const int activeBuzzerPin = 11;
int buzzerTone = 12000;
int buzzerValue = 0;

//   Variables used by the button
const int pushButton = 2;
int pushButtonState = 1;

void setup() {
  pinMode(passiveBuzzerPin, INPUT);
  pinMode(activeBuzzerPin, OUTPUT);
  pinMode(pushButton, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  //   gets the value of a knock
  speakerValue = analogRead(passiveBuzzerPin);
  if(speakerValue != 0)
    Serial.println(speakerValue);
  //    checks if the knock value is greater then the threshold
  if(speakerValue > threshold) {
    Serial.println("Knock!");
    currentMillis = millis();
    knockValue = 1;
  }
  //     if 5 seconds have passed and there was detected a knock play a tone 
  if((currentMillis - previousMillis >= intervalForBuzz) && (knockValue == 1)) {
     previousMillis = currentMillis;
     tone(activeBuzzerPin, buzzerTone);
  }
  pushButtonState = digitalRead(pushButton);
  //        if the button is pressed stop the tune
  if(!pushButtonState) {
    Serial.println("BUTTON PRESSED!");
    noTone(activeBuzzerPin);
    knockValue = 0;
    pushButtonState = !pushButtonState;
  }
}
