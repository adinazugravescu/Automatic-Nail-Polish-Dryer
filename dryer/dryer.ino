#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define FAN_PIN 3           // Fan controlled via MOSFET on pin D3 (PWM-capable)
#define SENSOR_PIN 2        // IR sensor output connected to pin D2

LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD with I2C interface at address 0x27

bool drying = false;
unsigned long startTime = 0;
const unsigned long dryingDuration = 60000; // Duration: 60 seconds

void setup() {
  pinMode(FAN_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);
  analogWrite(FAN_PIN, 0);         // Ensure fan is off initially (PWM 0%)

  lcd.init();                      
  lcd.backlight();                 
  lcd.setCursor(5, 0);
  lcd.print("Ready");              // Initial status
}

void loop() {
  // If not currently drying and a hand is detected
  if (!drying && digitalRead(SENSOR_PIN) == LOW) {
    drying = true;
    startTime = millis();
    analogWrite(FAN_PIN, 200);     // Start fan using PWM
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Drying started");
  }

  if (drying) {
    unsigned long elapsed = millis() - startTime;
    unsigned long remaining = (dryingDuration - elapsed) / 1000;

    lcd.setCursor(4, 1);
    lcd.print("Time: ");
    lcd.print(remaining);
    lcd.print("s   ");

    // Stop fan when countdown finishes
    if (elapsed >= dryingDuration) {
      analogWrite(FAN_PIN, 0);     // Stop fan via PWM
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Done!");
      delay(7000);                 
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Ready");
      drying = false;
    }
  }
}
