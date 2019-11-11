const int passiveBuzzerPin = A4;
int speakerValue = 0;
const int threshold = 20;

unsigned long previousMillis = 0;
const long intervalForBuzz = 5000;

const int activeBuzzerPin = 11;
int buzzerTone = 12000;
int buzzerValue = 0;

const int pushButton = 2;
int pushButtonState = 1;

int knockValue = 0;

void setup() {
  pinMode(passiveBuzzerPin, INPUT);
  pinMode(activeBuzzerPin, OUTPUT);
  pinMode(pushButton, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  speakerValue = analogRead(passiveBuzzerPin);
  if(speakerValue != 0)
    Serial.println(speakerValue);
  if(speakerValue > threshold) {
    Serial.println("Knock!");
    knockValue = 1;
  }
  if((currentMillis - previousMillis >= intervalForBuzz) && (knockValue == 1)) {
     previousMillis = currentMillis;
     tone(activeBuzzerPin, buzzerTone);
  }
  pushButtonState = digitalRead(pushButton);
  
  if(!pushButton) {
    Serial.println("BUTTON PRESSED!");
    noTone(activeBuzzerPin);
    knockValue = 0;
  }
}
