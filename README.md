# STARBiE 🌱

### Smart Plant & Environmental Monitor

STARBiE is a simple and intelligent Arduino-based environmental monitoring device designed to help monitor plant and environmental conditions in real time.

It reads **temperature and soil moisture**, calculates a **health score**, detects abnormal conditions, provides **recommendations**, monitors **environmental trends**, and detects if the device is tilted.

## ✨ Features

* 🌡️ Temperature monitoring using TMP36
* 🌱 Soil moisture monitoring
* ❤️ Environmental health score
* ⚠️ Automatic alerts
* 💡 Simple recommendations
* 📈 Temperature and soil trend detection
* 📐 Tilt detection
* 💾 EEPROM storage for tilt events
* 🔘 One-button navigation
* 📟 16×2 I²C LCD display
* ⚡ Non-blocking `millis()` based operation
* 🔧 Designed for simplicity and reliability

## 🧠 How STARBiE Works

```text
Sensors
   ↓
Arduino Uno
   ↓
Data Processing
   ↓
Health & Alert Analysis
   ↓
Recommendation
   ↓
LCD Display
```

STARBiE continuously reads the sensors and converts the raw data into information that is easier to understand.

For example:

```text
Soil < 20%
      ↓
  SOIL DRY
      ↓
 WATER PLANT
```

If the device is tilted:

```text
Tilt detected
      ↓
TILT ALERT
      ↓
Tilt event saved in EEPROM
```

## 🛠️ Hardware

| Component            | Purpose                 |
| -------------------- | ----------------------- |
| Arduino Uno R3       | Main controller         |
| TMP36                | Temperature sensing     |
| Soil Moisture Sensor | Soil monitoring         |
| Tilt Sensor          | Tilt detection          |
| 16×2 I²C LCD         | User interface          |
| Push Button          | Navigation              |
| EEPROM               | Stores tilt-event count |

## 🔌 Pin Connections

| Component      | Arduino |
| -------------- | ------- |
| TMP36 OUT      | A0      |
| Soil Sensor AO | A1      |
| Button         | D3      |
| Tilt Sensor    | D7      |
| LCD SDA        | SDA     |
| LCD SCL        | SCL     |
| LCD VCC        | 5V      |
| LCD GND        | GND     |

The button and tilt sensor use the Arduino's internal pull-up resistors, so no external 10kΩ resistor is required.

## 🖥️ LCD Screens

### 1. Dashboard

```text
T:24.8C S:62%
HEALTH:85%
```

### 2. Status & Recommendation

Examples:

```text
SOIL DRY
WATER PLANT
```

or

```text
STATUS OK
ENVIRONMENT
```

### 3. Trends

```text
TEMP RISING
SOIL DRYING
```

### 4. Tilt Memory

```text
TILT EVENTS
5 TIMES
```

### 5. Sensor Values

```text
TEMP 24.8C
SOIL 62%
```

Press the button to move between screens.

## 💾 EEPROM

STARBiE stores the number of detected tilt events in the Arduino's EEPROM.

This means the count remains available even after the Arduino is powered off and restarted.

## 📚 Libraries

The project uses:

```cpp
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_LiquidCrystal.h>
```

Install the **Adafruit LiquidCrystal** library if required by your Arduino/Tinkercad environment.

## 🚀 Getting Started

1. Connect the components according to the wiring table.
2. Open the STARBiE Arduino code.
3. Select **Arduino Uno**.
4. Upload the code.
5. Power STARBiE.
6. Use the button to navigate through the screens.
7. Test the temperature, soil moisture, and tilt sensors.

## 🎯 Project Goal

The goal of STARBiE is not to make a complicated system with hundreds of features.

Instead, it focuses on one principle:

> **Turn simple sensor data into useful information and actions.**

STARBiE combines sensing, basic intelligence, alerts, recommendations, and memory into a compact and easy-to-understand device.

## 🔮 Future Improvements

Possible future versions could include:

* Wireless connectivity
* Mobile application
* More environmental sensors
* Automatic plant watering
* Solar power
* Long-term data logging
* More advanced environmental analysis

## 📄 License

This project is created as an educational and experimental hardware project.

**Made with curiosity, electronics, and a goal of building smarter technology. 🌱⚡**
