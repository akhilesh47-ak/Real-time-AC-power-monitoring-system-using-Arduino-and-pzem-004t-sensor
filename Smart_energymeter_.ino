#include <SoftwareSerial.h>
#include <PZEM004Tv30.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ===== PIN DEFINITIONS (MATCHING YOUR WIRING) =====
#define PZEM_RX_PIN 8 // Arduino RX (listens) – connect to PZEM TX
#define PZEM_TX_PIN 9 // Arduino TX (speaks) – connect to PZEM RX

// ===== OBJECTS =====
SoftwareSerial pzemSWSerial(PZEM_RX_PIN, PZEM_TX_PIN);
PZEM004Tv30 pzem(pzemSWSerial);

// LCD I2C – try 0x27 or 0x3F if this doesn't work
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===== VARIABLES =====
float voltage, current, power, energy, frequency, pf;
unsigned long lastDisplayTime = 0;
const unsigned long DISPLAY_INTERVAL = 2000;

void setup() {
  Serial.begin(9600);

  // For your library version, just call begin() with no arguments
  lcd.begin();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("PZEM Meter Ready");
  delay(1500);
}

void loop() {
  voltage = pzem.voltage();
  current = pzem.current();
  power = pzem.power();
  energy = pzem.energy();
  frequency = pzem.frequency();
  pf = pzem.pf();
  
  if (millis() - lastDisplayTime >= DISPLAY_INTERVAL) {
    lcd.clear();
    
    if (isnan(voltage)) {
      lcd.setCursor(0, 0);
      lcd.print("PZEM Error!");
      lcd.setCursor(0, 1);
      lcd.print("Check wiring");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("V:");
      lcd.print(voltage, 1);
      lcd.print("V I:");
      lcd.print(current, 2);
      lcd.print("A");
      
      lcd.setCursor(0, 1);
      lcd.print("P:");
      lcd.print(power, 1);
      lcd.print("W PF:");
      lcd.print(pf, 2);
    }
    
    lastDisplayTime = millis();
  }
  
  // Debug output
  Serial.print("V: "); Serial.print(voltage, 1);
  Serial.print("V I: "); Serial.print(current, 2);
  Serial.print("A P: "); Serial.print(power, 1);
  Serial.print("W PF: "); Serial.println(pf, 2);
  
  delay(100);
}