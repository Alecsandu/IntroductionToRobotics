## Laboratory Homeworks for the Introduction to Robotics Courses

### 1st Homework
**fileName**: analogRGBControl
**description**: This program will read the values from 3 different potentiometers with the help of analogRead() function.
After that it will map those values using the map(potValue, 0, 1023, 0, 255) because the read values are between 0 and 1023 and 
the written values are between 0 and 255, so in the end the value that we will use will be between 0 and 255.
Finally, we will give these three values as arguments to the function void setColor(int color1, int color2, int color3) which
will control the colors of a RGB LED with the help of analogWrite() function.
### 2nd Homework
**fileName**: knockDetectorBuzzer
**description**: This program starts after a knock and after 5 second it plays a tone that can be stopped with the press of a button.
### 3rd Homework
**fileName**: 4-7segmentsDisplayWithJoystick
**description**: This program uses a 4 digit 7-segment display and a joystick. The digits can be increased or decreased with the help of the joystick. This program has three states : 1-locked in(when the dp starts blinking and you can change the value of the selected digit), selected (when the dp is lit but can't change the value of the digit) and not selected(the value is shown but the dp isn't lit or blinking).
