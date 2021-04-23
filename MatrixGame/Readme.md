# LED MATRIX GAME - Agility SNAKE

## The game / Story
You play as "Snacko" a lazy snake that has awakened after a long and cold winter. Now he is hungry and wants you to direct him to the prey as fast as you can because after some time the prey will notice him and disappear.

Either way, if you get it or not, you will have to direct Snacko to a new food spot until he will be bored and go back to sleep.

So, if you want to achive a great score you will have to move him quick and watch out for the walls, because if you make him hit them too many times he will get injured and eventually go back to sleep.

## How to play
In order to start the game you will have to select the start option on the LCD screen and press the button of the joystick.

After that with the help of the X and Y axis of the joystick you will change Snacko's position one at a time either up, down, left or right, to get the food until the time is up or if you lose all your lives.

After the time is up or your Snacko died you will see your score and will be asked to press the button in order to get back to the main menu.

## Main menu (LCD's screen)
It has 4 options to choose from:
### 1.Start
This is the first option that will let you play the game. Before selecting it you can also select what level do you want to start playing.
### 2.Setup
Here the player can choose the level that he wants to play, the name that he will have and can also reset the highscore(you can make this only if you know the secret move, moving the Y axis to the right)
### 3.HighScore
This will show you the name of the player that achieved the highest score and his/her score.
### 4.Info
Here you can see the name of the game, my name (the creator of the game), my GitHub link and the name of the course that I attended to.

## Other specifications
- If you can beat the highest score you will get one extra life
- The highest score will be saved on EEPROM
- Depending on the level you choosed, the amount of seconds that you have in order to get to the food spot will vary

## Technical requirements / Hardware used
The game is displayed on on a 8x8 matrix using a MAX7219 driver.

For the controls it is used a joystick.

While you are playing the game you can see on the LCD screen the number of lives, the level that you selected to play, score and how much time is left.

You can control the LCD's contrast with the help of a potentiometer.

The score is calculated by adding to the current score, each time you get to a food spot, a random number, that was generated when you started the level, multiplicated with the value of the level.

So if you want to beat the highest score you need to have both luck and skill.

### Hardware
- 1 * 8x8 LED matrix <br/>
- 1 * LCD <br/>
- 1 * Joystick<br/>
- 1 * MAX7219 Driver<br/>
- 1 * Potentiometer<br/>
- Wires<br/>
- Resistors (220 Ohm & 10k Ohm)<br/>
- Arduino<br/>
- Connector cable<br/>
- 1 * Breadbord<br/>
- 1 * Mini Breadbords<br/> 

## Game presentation
Link to the video:https://youtu.be/unNmHX2CIIg

Link to the image of the project:https://drive.google.com/file/d/1sljk9I1atlTDdTxXl9j89HE8bjMNos2N/view?usp=sharing
