#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Native I2C display dependency

// --- SIMULATION PATH CONTROL NODES ---
const int LEFT_TRIG = 2;    const int LEFT_ECHO = 3;
const int RIGHT_TRIG = 4;   const int RIGHT_ECHO = 5;
const int BUZZER = 6;
const int IN1 = 8;          const int IN2 = 9;         
const int IN3 = 10;         const int IN4 = 11;
const int CENTER_TRIG = 12; const int CENTER_ECHO = 13; // Bypassing missing firmware blocks

const int CRITICAL_BRAKE_DISTANCE = 35; 
LiquidCrystal_I2C lcd(0x27, 16, 2); 

int sampleSonar(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long pulseDuration = pulseIn(echoPin, HIGH, 25000); 
  if (pulseDuration == 0) return 400;                 
  return pulseDuration * 0.034 / 2;
}

void executeMotorCommand(int m1, int m2, int m3, int m4) {
  digitalWrite(IN1, m1); digitalWrite(IN2, m2);
  digitalWrite(IN3, m3); digitalWrite(IN4, m4);
}

void setup() {
  Wire.begin(); 
  lcd.init(); 
  lcd.backlight();
  
  pinMode(LEFT_TRIG, OUTPUT);   pinMode(LEFT_ECHO, INPUT);
  pinMode(RIGHT_TRIG, OUTPUT);  pinMode(RIGHT_ECHO, INPUT);
  pinMode(CENTER_TRIG, OUTPUT); pinMode(CENTER_ECHO, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  
  lcd.print("Autonex Active...");
  delay(1000);
  lcd.clear();
}

void loop() {
  int centerDistance = sampleSonar(CENTER_TRIG, CENTER_ECHO); 
  int leftDistance = sampleSonar(LEFT_TRIG, LEFT_ECHO);
  int rightDistance = sampleSonar(RIGHT_TRIG, RIGHT_ECHO);

  lcd.setCursor(0, 0); 
  lcd.print("C:"); lcd.print(centerDistance); 
  lcd.print(" L:"); lcd.print(leftDistance); 
  lcd.print(" R:"); lcd.print(rightDistance); 
  lcd.print("   ");

  if (centerDistance < CRITICAL_BRAKE_DISTANCE || leftDistance < CRITICAL_BRAKE_DISTANCE || rightDistance < CRITICAL_BRAKE_DISTANCE) {
    executeMotorCommand(LOW, LOW, LOW, LOW); // Trigger Emergency Braking
    digitalWrite(BUZZER, HIGH);
    lcd.setCursor(0, 1); lcd.print("HAZARD DETECTED ");
    delay(400);
    
    if (leftDistance > rightDistance) {
      lcd.setCursor(0, 1); lcd.print("Evasion: LEFT   ");
      executeMotorCommand(LOW, HIGH, HIGH, LOW); 
    } else {
      lcd.setCursor(0, 1); lcd.print("Evasion: RIGHT  ");
      executeMotorCommand(HIGH, LOW, LOW, HIGH); 
    }
    delay(650); 
    executeMotorCommand(LOW, LOW, LOW, LOW);
    digitalWrite(BUZZER, LOW);
  } else {
    lcd.setCursor(0, 1); lcd.print("PATH CLEAR - NAV");
    executeMotorCommand(HIGH, LOW, HIGH, LOW); 
  }
  delay(50);
}