## Laboratory Homeworks for the Introduction to Robotics Courses

### [1st Homework](analogRGBControl.ino)
**FileName**: analogRGBControl.ino
<br />
**Description**: This program will read the values from 3 different potentiometers with the help of analogRead() function.
After that it will map those values using the map(potValue, 0, 1023, 0, 255) because the read values are between 0 and 1023 and 
the written values are between 0 and 255, so in the end the value that we will use will be between 0 and 255.
<br />
Finally, we will give these three values as arguments to the function void setColor(int color1, int color2, int color3) which
will control the colors of a RGB LED with the help of analogWrite() function.
<br />
<hr />

### [2nd Homework](knockDetectorBuzzer.ino)
**FileName**: knockDetectorBuzzer.ino
<br />
**Description**: This program starts after a knock and after 5 second it plays a tone that can be stopped with the press of a button.
<br />
<hr />

### [3rd Homework](4-7segmentsDisplayWithJoystick.ino)
**FileName**: 4-7segmentsDisplayWithJoystick.ino
<br />
**Description**: This program uses a 4 digit 7-segment display and a joystick. The digits can be increased or decreased with the help of the joystick.
<br />
This program has three states :
1. locked in (when the dp starts blinking and you can change the value of the selected digit)
2. selected (when the dp is lit but can't change the value of the digit)
3. not selected (the value is shown but the dp isn't lit or blinking)
<a />
<hr />

### [4th Homework](gameMenu.ino)
**FileName**: gameMenu.ino
<br />
**Demo on youtube**: [IntroductionToRobotics GameMenu Arduino](https://www.youtube.com/watch?v=rMigjgztc5U)
<br />
**Description**: This program simulates the menu of a game where you can start the game, change the settings and see the highscore.
In the setup you can change the level with the joystick and the name via the console from the Arduino IDE console.
<br />
The highest score is stored with the help of <b>EPROM</b>. When the game starts you need to wait and after some seconds you will have to press the button from the joystick for it to stop. If the currentscore is higher than the highest score it will update and remain stored.
