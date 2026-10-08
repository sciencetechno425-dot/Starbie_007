#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(0);

// ---------- PINS ----------
const byte TEMP_PIN = A0;
const byte SOIL_PIN = A1;
const byte BUTTON_PIN = 3;
const byte TILT_PIN = 7;

// ---------- DATA ----------
float temperature = 0;
int soil = 0;
int health = 100;

float oldTemp = 0;
int oldSoil = 0;

bool tempRising = false;
bool tempFalling = false;
bool soilDrying = false;
bool soilWetting = false;

int tiltEvents = 0;
bool lastTilt = HIGH;

byte screen = 0;
const byte SCREENS = 5;

// ---------- TIMERS ----------
unsigned long lastRead = 0;
unsigned long lastDisplay = 0;
unsigned long lastButton = 0;

const unsigned long READ_TIME = 500;
const unsigned long DISPLAY_TIME = 300;
const unsigned long DEBOUNCE = 250;

// ---------- SETUP ----------
void setup()
{
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(TILT_PIN, INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.setBacklight(1);

  EEPROM.get(0, tiltEvents);

  if (tiltEvents < 0 || tiltEvents > 10000)
    tiltEvents = 0;

  lcd.setCursor(0, 0);
  lcd.print("STARBiE");

  lcd.setCursor(0, 1);
  lcd.print("SYSTEM READY");
}

// ---------- LOOP ----------
void loop()
{
  unsigned long now = millis();

  if (now - lastRead >= READ_TIME)
  {
    lastRead = now;

    readSensors();
    calculateHealth();
    detectTrend();
    checkTilt();
  }

  checkButton();

  if (now - lastDisplay >= DISPLAY_TIME)
  {
    lastDisplay = now;
    showScreen();
  }
}

// ---------- SENSOR READING ----------
void readSensors()
{
  int rawTemp = analogRead(TEMP_PIN);

  float voltage = rawTemp * (5.0 / 1023.0);

  temperature = (voltage - 0.5) * 100.0;

  int rawSoil = analogRead(SOIL_PIN);

  soil = map(rawSoil, 1023, 0, 0, 100);
  soil = constrain(soil, 0, 100);
}

// ---------- HEALTH ----------
void calculateHealth()
{
  health = 100;

  // Temperature
  if (temperature < 10 || temperature > 40)
    health -= 35;
  else if (temperature < 15 || temperature > 35)
    health -= 15;

  // Soil
  if (soil < 20)
    health -= 35;
  else if (soil < 35)
    health -= 15;

  // Tilt
  if (digitalRead(TILT_PIN) == LOW)
    health -= 10;

  health = constrain(health, 0, 100);
}

// ---------- TREND ----------
void detectTrend()
{
  if (temperature > oldTemp + 1)
  {
    tempRising = true;
    tempFalling = false;
  }
  else if (temperature < oldTemp - 1)
  {
    tempRising = false;
    tempFalling = true;
  }
  else
  {
    tempRising = false;
    tempFalling = false;
  }

  if (soil < oldSoil - 3)
  {
    soilDrying = true;
    soilWetting = false;
  }
  else if (soil > oldSoil + 3)
  {
    soilDrying = false;
    soilWetting = true;
  }
  else
  {
    soilDrying = false;
    soilWetting = false;
  }

  oldTemp = temperature;
  oldSoil = soil;
}

// ---------- TILT ----------
void checkTilt()
{
  bool tilt = digitalRead(TILT_PIN);

  if (lastTilt == HIGH && tilt == LOW)
  {
    tiltEvents++;

    EEPROM.put(0, tiltEvents);
  }

  lastTilt = tilt;
}

// ---------- BUTTON ----------
void checkButton()
{
  if (digitalRead(BUTTON_PIN) == LOW &&
      millis() - lastButton > DEBOUNCE)
  {
    lastButton = millis();

    screen++;

    if (screen >= SCREENS)
      screen = 0;
  }
}

// ---------- DISPLAY ----------
void showScreen()
{
  lcd.clear();

  // SCREEN 0
  // MAIN DASHBOARD
  if (screen == 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temperature, 1);
    lcd.print("C S:");
    lcd.print(soil);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("HEALTH:");
    lcd.print(health);
    lcd.print("%");
  }

  // SCREEN 1
  // STATUS + RECOMMENDATION
  else if (screen == 1)
  {
    lcd.setCursor(0, 0);

    if (temperature > 40)
    {
      lcd.print("HIGH TEMP");
      lcd.setCursor(0, 1);
      lcd.print("COOL ENV");
    }
    else if (temperature < 10)
    {
      lcd.print("LOW TEMP");
      lcd.setCursor(0, 1);
      lcd.print("CHECK ENV");
    }
    else if (soil < 20)
    {
      lcd.print("SOIL DRY");
      lcd.setCursor(0, 1);
      lcd.print("WATER PLANT");
    }
    else if (soil > 85)
    {
      lcd.print("SOIL WET");
      lcd.setCursor(0, 1);
      lcd.print("CHECK WATER");
    }
    else if (digitalRead(TILT_PIN) == LOW)
    {
      lcd.print("TILT ALERT");
      lcd.setCursor(0, 1);
      lcd.print("CHECK STARBiE");
    }
    else
    {
      lcd.print("STATUS OK");
      lcd.setCursor(0, 1);
      lcd.print("ENVIRONMENT");
    }
  }

  // SCREEN 2
  // TREND
  else if (screen == 2)
  {
    lcd.setCursor(0, 0);

    if (tempRising)
      lcd.print("TEMP RISING");
    else if (tempFalling)
      lcd.print("TEMP FALLING");
    else
      lcd.print("TEMP STABLE");

    lcd.setCursor(0, 1);

    if (soilDrying)
      lcd.print("SOIL DRYING");
    else if (soilWetting)
      lcd.print("SOIL WETTING");
    else
      lcd.print("SOIL STABLE");
  }

  // SCREEN 3
  // TILT MEMORY
  else if (screen == 3)
  {
    lcd.setCursor(0, 0);
    lcd.print("TILT EVENTS");

    lcd.setCursor(0, 1);
    lcd.print(tiltEvents);
    lcd.print(" TIMES");

    if (digitalRead(TILT_PIN) == LOW)
    {
      lcd.setCursor(10, 1);
      lcd.print("TILT!");
    }
  }

  // SCREEN 4
  // SENSOR VALUES
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("TEMP ");
    lcd.print(temperature, 1);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("SOIL ");
    lcd.print(soil);
    lcd.print("%");
  }

  // Serial monitor
  Serial.print("T=");
  Serial.print(temperature, 1);

  Serial.print("C | Soil=");
  Serial.print(soil);

  Serial.print("% | Health=");
  Serial.print(health);

  Serial.print("% | Tilt=");
  Serial.println(tiltEvents);
}