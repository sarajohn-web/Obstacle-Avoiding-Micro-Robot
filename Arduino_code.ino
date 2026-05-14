#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>

// Sensor Pin
const int sigPin = 7;

// Initialize Adafruit I2C LCD using TinkerCad's default I2C address
// Try 0x20 first. If the screen remains blank, change 0x20 to 0x27.
Adafruit_LiquidCrystal lcd(0x20); 

// Motor Control Pins
const int leftForward = 8;
const int leftBackward = 10;
const int rightForward = 5;
const int rightBackward = 4;
const int enaSpeed = 9;  // Speed control left (PWM)
const int enbSpeed = 6;  // Speed control right (PWM)

void setup() {
  // Initialize Adafruit LCD (16 columns, 2 rows)
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH); // Turn on the screen backlight
  
  // Initialize Motor Pin Configurations
  pinMode(leftForward, OUTPUT);
  pinMode(leftBackward, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightBackward, OUTPUT);
  pinMode(enaSpeed, OUTPUT);
  pinMode(enbSpeed, OUTPUT);
}

void loop() {
  long duration;
  float distance;

  // Trigger pulse from 3-pin ultrasonic sensor via SIG pin
  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(sigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sigPin, LOW);

  // Read echo duration
  pinMode(sigPin, INPUT);
  duration = pulseIn(sigPin, HIGH);
  distance = (duration * 0.0343) / 2;

  // Navigation Logic
  if (distance > 30) {
    // Path is unobstructed -> Move Forward
    analogWrite(enaSpeed, 200); // Set motor speed (0-255)
    analogWrite(enbSpeed, 200);
    digitalWrite(leftForward, HIGH);
    digitalWrite(leftBackward, LOW);
    digitalWrite(rightForward, HIGH);
    digitalWrite(rightBackward, LOW);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Robot:");
    lcd.setCursor(0, 1);
    lcd.print("Moving Forward");
  } 
  else {
    // Obstacle detected -> Stop, Reverse, and Turn Left
    // 1. Full Stop
    digitalWrite(leftForward, LOW);
    digitalWrite(rightForward, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("OBSTACLE AHEAD!");
    lcd.setCursor(0, 1);
    lcd.print("Dist: "); lcd.print(distance); lcd.print("cm");
    delay(500);

    // 2. Reverse slightly
    digitalWrite(leftBackward, HIGH);
    digitalWrite(rightBackward, HIGH);
    delay(600);
    digitalWrite(leftBackward, LOW);
    digitalWrite(rightBackward, LOW);

    // 3. Perform Left Escape Turn
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Executing Turn");
    digitalWrite(leftBackward, HIGH); 
    digitalWrite(rightForward, HIGH); 
    delay(800);                       
    
    digitalWrite(leftBackward, LOW);
    digitalWrite(rightForward, LOW);
  }
  
  delay(100); // Refresh buffer loop delay
}
