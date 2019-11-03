const int redIn = A0;
const int greenIn = A1;
const int blueIn = A2;
const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;
int redValue = 0;
int greenValue = 0;
int blueValue = 0;

void setup() {
  pinMode(redIn, INPUT);
  pinMode(greenIn, INPUT);
  pinMode(blueIn, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  redValue = analogRead(redIn);
  greenValue = analogRead(greenIn);
  blueValue = analogRead(blueIn);
  redValue = map(redValue, 0, 1023, 0, 255);
  greenValue = map(greenValue, 0, 1023, 0, 255);
  blueValue = map(blueValue, 0, 1023, 0, 255);
  setColor(redValue, greenValue, blueValue);
}

void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
