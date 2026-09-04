#include<Wire.h>
#include <LiquidCrystal_I2C.h>

//Sends out a sound wave and listens for the sound wave to come back
const int LEFT_TRIG = 2;
const int LEFT_ECHO = 3; 
const int RIGHT_TRIG = 4;
const int RIGHT_ECHO = 5;

const int BUZZER = 6;

//control the direction and speed of our motors
const int IN1 = 8;
const int IN2 = 9;         
const int IN3 = 10;         
const int IN4 = 11;

//Sensor is in the middle
const int CENTER_TRIG = 12;
const int CENTER_ECHO = 13;

// If an object is closer than 35 cm then stop
const int BRAKE_DISTANCE = 35;

//Intialize LCD , 2 lines , 16 char per line
LiquidCrystal_I2C lcd(0x27, 16, 2); 

//Function to measure distance using ultrasonic sensors
int sampleSonar(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long pulseDuration = pulseIn(echoPin, HIGH, 25000); 

  if (pulseDuration == 0) return 400;
  return pulseDuration * 0.034 / 2;
}

//Function to control motors
void executeMotorCommand(int m1, int m2, int m3, int m4) 
{
  digitalWrite(IN1, m1);
  digitalWrite(IN2, m2);
  digitalWrite(IN3, m3);
  digitalWrite(IN4, m4);
}

void setup()
{
  Wire.begin();
  lcd.init();
  lcd.backlight();

  // Trig pins, we send signals from them
  // Echo pins, we listen for signals on them

  pinMode(LEFT_TRIG, OUTPUT);   
  pinMode(LEFT_ECHO, INPUT);
  pinMode(RIGHT_TRIG, OUTPUT);  
  pinMode(RIGHT_ECHO, INPUT);
  pinMode(CENTER_TRIG, OUTPUT); 
  pinMode(CENTER_ECHO, INPUT);
  pinMode(BUZZER, OUTPUT);

  //Motors control pins
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT);

  // Welcome message on the LCD
  lcd.print("Autonex Active...");
  delay(1000); // Wait for 1 second.
  lcd.clear(); // Clear the screen.
}

void loop()
{
  //Read distances from all three ultrasonic sensors
  int centerDistance = sampleSonar(CENTER_TRIG, CENTER_ECHO);
  int leftDistance = sampleSonar(LEFT_TRIG, LEFT_ECHO);
  int rightDistance = sampleSonar(RIGHT_TRIG, RIGHT_ECHO);

  //Display the sensor readings on the LCD screen
  lcd.setCursor(0, 0);
  lcd.print("C:"); lcd.print(centerDistance);
  lcd.print(" L:"); lcd.print(leftDistance);
  lcd.print(" R:"); lcd.print(rightDistance); 
  lcd.print("   ");

  //Emergency braking if object is too close
  if (centerDistance < BRAKE_DISTANCE || leftDistance < BRAKE_DISTANCE || rightDistance < BRAKE_DISTANCE)
  {
    executeMotorCommand(LOW, LOW, LOW, LOW);
    digitalWrite(BUZZER, HIGH);
    lcd.setCursor(0, 1); lcd.print("HAZARD DETECTED ");
    delay(400);

      // Decide which way to turn to avoid the obstacle
      if (leftDistance > rightDistance)
      {
        lcd.setCursor(0, 1); lcd.print("Evasion: LEFT   ");
        executeMotorCommand(LOW, HIGH, HIGH, LOW);
      }
      else
      {
        //if leftDistance and rightDistance are equal
        lcd.setCursor(0, 1); lcd.print("Evasion: RIGHT  ");
        executeMotorCommand(HIGH, LOW, LOW, HIGH);
      }
      delay(650);
      executeMotorCommand(LOW, LOW, LOW, LOW);
      digitalWrite(BUZZER, LOW);
  }
  else
  {
    //If no obstacle is in range 
    lcd.setCursor(0, 1); lcd.print("PATH CLEAR");
    executeMotorCommand(HIGH, LOW, HIGH, LOW);
  }
    delay(50);
}
