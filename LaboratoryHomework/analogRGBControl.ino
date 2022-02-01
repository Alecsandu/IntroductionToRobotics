const int redIn = A0;
const int greenIn = A1;
const int blueIn = A2;
const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;
int redValue = 0;
int greenValue = 0;
int blueValue = 0;

/*
  In setup it is configured each pin to behave either as an input or an output
*/
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
  /*
    After we read the values from the 3 potentiometers, the map function is used
    because stored values are between 0-1023 and the written values must be
    between 0-255
  */
  redValue = map(redValue, 0, 1023, 0, 255);
  greenValue = map(greenValue, 0, 1023, 0, 255);
  blueValue = map(blueValue, 0, 1023, 0, 255);
  setColor(redValue, greenValue, blueValue);
}

/*
  This function is used to control each color channel of the RGB LED
*/
void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
