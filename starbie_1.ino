#include <LiquidCrystal_I2C.h>

// =====================================================
// STARBiE V1
// Arduino Uno + LCD + Temperature + Soil + Tilt + 4 Buttons
// =====================================================

LiquidCrystal_I2C lcd(0x20, 16, 2);

// ---------- PINS ----------

#define TEMP_PIN A0
#define SOIL_PIN A1

#define BUTTON1 3
#define BUTTON2 4
#define BUTTON3 5
#define BUTTON4 6

#define TILT_PIN 7


// ---------- VARIABLES ----------

float temperature = 0;
int soilMoisture = 0;


// =====================================================
// TEMPERATURE
// TMP36
// =====================================================

float readTemperature() {

  int raw = analogRead(TEMP_PIN);

  float voltage = raw * (5.0 / 1023.0);

  float tempC = (voltage - 0.5) * 100.0;

  return tempC;
}


// =====================================================
// SOIL MOISTURE
// =====================================================

int readSoilMoisture() {

  int raw = analogRead(SOIL_PIN);

  // Tinkercad soil sensor:
  // Higher reading = drier soil
  // Lower reading = wetter soil

  int moisture = map(raw, 1023, 0, 0, 100);

  moisture = constrain(moisture, 0, 100);

  return moisture;
}


// =====================================================
// READ ALL SENSORS
// =====================================================

void readSensors() {

  temperature = readTemperature();

  soilMoisture = readSoilMoisture();
}


// =====================================================
// STARTUP
// =====================================================

void startup() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("    STARBiE");

  lcd.setCursor(0, 1);
  lcd.print("System Starting");

  Serial.println("================================");
  Serial.println("       STARBiE V1");
  Serial.println("       SYSTEM START");
  Serial.println("================================");

  delay(2000);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("STARBiE ONLINE");

  lcd.setCursor(0, 1);
  lcd.print("Ready!");

  Serial.println("STARBiE: ONLINE");

  delay(1500);
}


// =====================================================
// ENVIRONMENT SCREEN
// =====================================================

void environmentScreen() {

  readSensors();

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperature, 1);
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Soil:");
  lcd.print(soilMoisture);
  lcd.print("%");

  Serial.println();
  Serial.println("--- ENVIRONMENT ---");

  Serial.print("Temperature: ");
  Serial.print(temperature, 1);
  Serial.println(" C");

  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisture);
  Serial.println("%");

  delay(2000);
}


// =====================================================
// ACTIVITY SCREEN
// =====================================================

void activityScreen() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("STARBiE ACTIVITY");

  lcd.setCursor(0, 1);

  if (digitalRead(TILT_PIN) == LOW) {

    lcd.print("MOVEMENT!");

    Serial.println("Activity: MOVEMENT");

  } else {

    lcd.print("STABLE");

    Serial.println("Activity: STABLE");
  }

  delay(2000);
}


// =====================================================
// STATUS SCREEN
// =====================================================

void statusScreen() {

  readSensors();

  lcd.clear();

  lcd.setCursor(0, 0);

  // Temperature warning
  if (temperature >= 35) {

    lcd.print("TEMP HIGH!");

    lcd.setCursor(0, 1);
    lcd.print("Check Temp");

    Serial.println("ALERT: HIGH TEMPERATURE");
  }

  // Soil warning
  else if (soilMoisture < 30) {

    lcd.print("SOIL DRY!");

    lcd.setCursor(0, 1);
    lcd.print("Water Needed");

    Serial.println("ALERT: SOIL DRY");
  }

  // Tilt warning
  else if (digitalRead(TILT_PIN) == LOW) {

    lcd.print("MOVEMENT!");

    lcd.setCursor(0, 1);
    lcd.print("Check Starbie");

    Serial.println("ALERT: MOVEMENT");
  }

  // Everything normal
  else {

    lcd.print("STARBiE");

    lcd.setCursor(0, 1);
    lcd.print("ALL SYSTEMS OK");

    Serial.println("STATUS: ALL SYSTEMS OK");
  }

  delay(2000);
}


// =====================================================
// MENU
// =====================================================

void menuScreen() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("B1 Environment");

  lcd.setCursor(0, 1);
  lcd.print("B2 Activity");

  delay(1800);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("B3 Status");

  lcd.setCursor(0, 1);
  lcd.print("B4 Menu");

  delay(1800);
}


// =====================================================
// HOME SCREEN
// =====================================================

void homeScreen() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("STARBiE");

  lcd.setCursor(0, 1);
  lcd.print("Press Button");

}


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(9600);

  // Buttons
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);

  // Tilt sensor
  pinMode(TILT_PIN, INPUT_PULLUP);

  // LCD
  lcd.init();
  lcd.backlight();

  startup();

  homeScreen();
}


// =====================================================
// MAIN LOOP
// =====================================================

void loop() {

  // ---------------- BUTTON 1 ----------------

  if (digitalRead(BUTTON1) == LOW) {

    delay(50);

    if (digitalRead(BUTTON1) == LOW) {

      environmentScreen();

      while (digitalRead(BUTTON1) == LOW);
    }
  }


  // ---------------- BUTTON 2 ----------------

  else if (digitalRead(BUTTON2) == LOW) {

    delay(50);

    if (digitalRead(BUTTON2) == LOW) {

      activityScreen();

      while (digitalRead(BUTTON2) == LOW);
    }
  }


  // ---------------- BUTTON 3 ----------------

  else if (digitalRead(BUTTON3) == LOW) {

    delay(50);

    if (digitalRead(BUTTON3) == LOW) {

      statusScreen();

      while (digitalRead(BUTTON3) == LOW);
    }
  }


  // ---------------- BUTTON 4 ----------------

  else if (digitalRead(BUTTON4) == LOW) {

    delay(50);

    if (digitalRead(BUTTON4) == LOW) {

      menuScreen();

      while (digitalRead(BUTTON4) == LOW);

      homeScreen();
    }
  }


  // ---------------- AUTOMATIC ALERT ----------------

  if (digitalRead(TILT_PIN) == LOW) {

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("! STARBiE ALERT!");

    lcd.setCursor(0, 1);
    lcd.print("Movement");

    Serial.println("!!! STARBiE ALERT: MOVEMENT !!!");

    delay(500);
  }

  delay(50);
}