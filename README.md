The provided Arduino code is fully correct, logically sound, and ready to be uploaded to your board.
It properly utilizes the pulseIn timeout to prevent freezing, pads the LCD display with spaces to
overwrite old characters cleanly, and effectively handles motor logic for evasion.

You can copy and paste the following simple description directly into your GitHub README.md file:
Autonex Active: Obstacle-Avoiding Robot
This project provides the brain for a smart, self-driving robot car. It uses three ultrasonic sensors 
(acting as eyes) to look around and safely navigate a room without crashing into walls, furniture, or other obstacles.

How It Works
Cruising: The robot constantly moves forward while scanning the area in front, to the left, and to the right.
Detecting Danger: If any sensor sees an object closer than 35 cm, the robot immediately stops its motors and sounds a warning buzzer.
Making Decisions: It quickly compares the distances on the left and right sides to figure out which direction has more open space.
Evasion: The robot automatically turns toward the safest, most open path and then continues its journey.
Live Updates: A small 16x2 LCD screen acts as a dashboard. It shows real-time distance measurements from all three sensors and prints
out what the robot is currently doing (like "HAZARD DETECTED" or "PATH CLEAR").
