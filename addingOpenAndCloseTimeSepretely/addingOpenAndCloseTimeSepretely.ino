#include <LiquidCrystal.h>
#include <EEPROM.h>

// Pin definitions for LCD
const int LCD_RS = 8;
const int LCD_ENABLE = 9;
const int LCD_D4 = 4;
const int LCD_D5 = 5;
const int LCD_D6 = 6;
const int LCD_D7 = 7;
const int BACKLIGHT = 10;
const int BUTTON_PIN = A0;  // Button connected to A0 pin
#define PNP_SENSOR_PIN A1
// Relay pin definitions
const int RELAY1_PIN = 2;
const int RELAY2_PIN = 3;
// Initialize the LCD with the interface pins
LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
// Button value thresholds
int buttonValues[5] = { 50, 200, 400, 600, 800 };
int currentSelection = 0;
bool inSettingsMode = false;
bool inMainMenu = true;
bool inSetR1Mode = false;
bool inSetR2Mode = false;
bool inSetR3Mode = false;
bool inStartMode = false;  // New flag for Start mode
bool isOff = false;
bool buttonPressed = false;
float setR1Time = 0.0;     // Time for Relay 1
float setDelayTime = 0.0;  // Time for Relay 2
float setR3Time = 0.0;  // Time for Relay 3
float stepSize = 0.1;      // Increment/Decrement step size
unsigned long relay1StartTime = 0;
unsigned long relay2StartTime = 0;
unsigned long relay3StartTime = 0;
bool isRelay1Active = false;             // Track if Relay 1 is active
bool isRelay2Active = false;             // Track if Relay 2 is active
bool flag = false;                       // Track if Relay 2 is active
unsigned long buttonPressStartTime = 0;  // Timer for long press
bool longPressDetected = false;          // Flag for long press
unsigned long relayDelayStartTime = 0;   // Timer for delay before Relay 2 activates
const int EXTERNAL_BUTTON_PIN = 1;       // D1 pin for the external button
float previousR1Time = -1.0;             // Initialize with a value that's unlikely to match the default value
// EEPROM addresses for saving times
const int EEPROM_ADDR_R1_TIME = 0;
const int EEPROM_ADDR_R2_TIME = 4;  // Different EEPROM address for Set R2
const int EEPROM_ADDR_R3_TIME = 8;  // Different EEPROM address for Set R2
const int A2_PIN = A2;              // Pin A2 for controlling an external device
// Menu items in the main menu
char* menuItems[2] = { "1: Settings" };  // Add Settings option
char* settingItems[4] = { "Set R1", "Set D.Time","SET R3", "Back" };
int currentSettingSelection = 0;
bool isCycleRunning = false;  // Flag to track if the cycle is running
const int D0_PIN = 0;         // Pin D0 to detect the short circuit
const int A3_PIN = A3;        // Pin A3 to toggle between HIGH and LOW
bool previousD0State = HIGH;  // To store the previous state of D0
bool currentStateA3 = HIGH;   // The current state of A3 (starting as LOW)

void setup() {
  // Serial.begin(9600);  // Initialize serial communication
  lcd.begin(16, 2);
  pinMode(BACKLIGHT, OUTPUT);
  digitalWrite(BACKLIGHT, HIGH);
  // Load saved times from EEPROM with a default value fallback
  EEPROM.get(EEPROM_ADDR_R1_TIME, setR1Time);
  EEPROM.get(EEPROM_ADDR_R2_TIME, setDelayTime);
  EEPROM.get(EEPROM_ADDR_R3_TIME, setR3Time);

  // If R1 or R2 time are uninitialized, assign default values
  if (isnan(setR1Time) || setR1Time < 0.0 || setR1Time > 99.9) {
    setR1Time = 0.0;
  }
  if (isnan(setDelayTime) || setDelayTime < 0.0 || setDelayTime > 99.9) {
    setDelayTime = 0.0;
  }
    if (isnan(setR3Time) || setR3Time < 0.0 || setR3Time > 99.9) {
    setR3Time = 0.0;
  }
  displayMenu();
}

void loop() {
  int buttonValue = analogRead(BUTTON_PIN);

  // Handle other modes based on buttonValue (main menu, settings, etc.)
  if (inMainMenu) {
    handleMainMenu(buttonValue);
  } else if (inSettingsMode) {
    handleSettingsMode(buttonValue);
  } else if (inSetR1Mode) {
    handleSetR1(buttonValue);
  } else if (inSetR2Mode) {
    handleDelayR2(buttonValue);
  }  else if (inSetR3Mode) {
    handleDelayR3(buttonValue);
  }else if (inStartMode) {
    // handleStartMode();
  }
  // Handle button press to navigate back
  if (buttonValue < buttonValues[0] && !buttonPressed) {  // Right button
    buttonPressed = true;
    if (inSetR1Mode) {
      inSetR1Mode = false;
      inSettingsMode = true;
      displaySettingsMenu();
    } else if (inSetR2Mode) {
      inSetR2Mode = false;
      inSettingsMode = true;
      displaySettingsMenu();
    }  else if (inSetR3Mode) {
      inSetR3Mode = false;
      inSettingsMode = true;
      displaySettingsMenu();
    }else if (inStartMode) {
      // Turn off both relays
      digitalWrite(RELAY1_PIN, LOW);
      digitalWrite(RELAY2_PIN, LOW);
      lcd.clear();
      lcd.print("Relays OFF");
      delay(1000);
      buttonPressed = false;
      inMainMenu = true;
      displayMenu();
    }
  }

  if (buttonValue >= buttonValues[0]) {
    buttonPressed = false;
  }
  delay(200);  // Small delay to avoid bouncing issues
}

void handleMainMenu(int buttonValue) {
  if (buttonValue < buttonValues[1] && !buttonPressed) {  // Down button pressed
    buttonPressed = true;
    if (currentSelection < 0) {  // Prevent scrolling beyond the only menu option
      currentSelection++;
      displayMenu();  // Refresh the display with updated selection
    }
  } else if (buttonValue < buttonValues[2] && !buttonPressed) {  // Up button pressed
    buttonPressed = true;
    if (currentSelection > 0) {  // Prevent scrolling beyond the only menu option
      currentSelection--;
      displayMenu();  // Refresh the display with updated selection
    }
  } else if (buttonValue < buttonValues[4] && !buttonPressed) {  // Select button pressed
    buttonPressed = true;
    if (currentSelection == 0) {  // If "Settings" is selected
      enterSettingsMode();        // Enter the Settings menu
    }
  }
}

void displayMenu() {
  lcd.clear();
  for (int i = 0; i < 2; i++) {
    lcd.setCursor(1, i);
    if (currentSelection + i < 2) {  // Changed from 4 to 2
      lcd.print(menuItems[currentSelection + i]);
    }
  }
  lcd.setCursor(0, 0);
  lcd.write('>');
}

void enterSettingsMode() {
  inSettingsMode = true;
  inMainMenu = false;
  currentSettingSelection = 0;
  displaySettingsMenu();
}

void handleSettingsMode(int buttonValue) {
  if (buttonValue < buttonValues[1] && !buttonPressed) {
    if (currentSettingSelection > 0) {
      currentSettingSelection--;
      displaySettingsMenu();
    }
  } else if (buttonValue < buttonValues[2] && !buttonPressed) {
    if (currentSettingSelection < 2) {
      currentSettingSelection++;
      displaySettingsMenu();
    }
  } else if (buttonValue < buttonValues[4] && !buttonPressed) {
    buttonPressed = true;
    if (currentSettingSelection == 3) {
      inSettingsMode = false;
      inMainMenu = true;
      displayMenu();
    } else if (currentSettingSelection == 0) {
      inSetR1Mode = true;
      inSettingsMode = false;
      displaySetR1();
    } else if (currentSettingSelection == 1) {
      inSetR2Mode = true;
      inSettingsMode = false;
      displaySetR2();
    }else if (currentSettingSelection == 2) {
      inSetR3Mode = true;
      inSettingsMode = false;
      displaySetR3();
    }
  }
}

// Add these declarations at the top with other global variables
unsigned long delayStartTime = 0;  // Timer for delay before Relay 2 activates
float remainingDelayTime = 0.0;    // Remaining delay time before Relay 2 turns on

// Display the settings menu
void displaySettingsMenu() {
  // Serial.println("display menu setting");
  lcd.clear();
  for (int i = 0; i < 3; i++) {
    lcd.setCursor(1, i);
    if (currentSettingSelection + i < 4) {
      lcd.print(settingItems[currentSettingSelection + i]);
    }
  }
  lcd.setCursor(0, 0);
  lcd.write('>');
}

// Display the R1 time setting screen
void displaySetR1() {
  lcd.clear();
  lcd.print("Set R1 Time:");
  lcd.setCursor(0, 1);
  lcd.print(setR1Time, 1);
  lcd.print(" sec");
}

void handleSetR1(int buttonValue) {
  if (buttonValue < buttonValues[1] && !buttonPressed) {
    setR1Time += stepSize;
    if (setR1Time > 99.9) setR1Time = 99.9;
    displaySetR1();
  } else if (buttonValue < buttonValues[2] && !buttonPressed) {
    setR1Time -= stepSize;
    if (setR1Time < 0.0) setR1Time = 0.0;
    displaySetR1();
  } else if (buttonValue < buttonValues[4] && !buttonPressed) {
    buttonPressed = true;
    EEPROM.put(EEPROM_ADDR_R1_TIME, setR1Time);
    lcd.clear();
    lcd.print("Successfully set");
    delay(2000);
    inSetR1Mode = false;
    inSettingsMode = true;
    displaySettingsMenu();
  }
}

void displaySetR2() {
  lcd.clear();
  lcd.print("Set Delay Time:");
  lcd.setCursor(0, 1);
  lcd.print(setDelayTime, 1);  // Display time with one decimal place
  lcd.print(" sec");
}

void handleDelayR2(int buttonValue) {
  if (buttonValue < buttonValues[1] && !buttonPressed) {  // Up button
    setDelayTime += stepSize;
    if (setDelayTime > 99.9) setDelayTime = 99.9;  // Max limit
    displaySetR2();
  } else if (buttonValue < buttonValues[2] && !buttonPressed) {  // Down button
    setDelayTime -= stepSize;
    if (setDelayTime < 0.0) setDelayTime = 0.0;  // Min limit
    displaySetR2();
  } else if (buttonValue < buttonValues[4] && !buttonPressed) {  // Select button
    buttonPressed = true;
    // Save time to EEPROM
    EEPROM.put(EEPROM_ADDR_R2_TIME, setDelayTime);
    lcd.clear();
    lcd.print("Successfully set");
    delay(2000);
    inSetR2Mode = false;
    inSettingsMode = true;
    displaySettingsMenu();
  }
}

void displaySetR3() {
  lcd.clear();
  lcd.print("Set R3 Time:");
  lcd.setCursor(0, 1);
  // lcd.print(setR3time, 1);  // Display time with one decimal place
  lcd.print(setR3Time, 1);  // Display time with one decimal place

  lcd.print(" sec");
}

void handleDelayR3(int buttonValue) {
  if (buttonValue < buttonValues[1] && !buttonPressed) {  // Up button
    setR3Time += stepSize;
    if (setR3Time > 99.9) setR3Time = 99.9;  // Max limit
    displaySetR3();
  } else if (buttonValue < buttonValues[2] && !buttonPressed) {  // Down button
    setR3Time -= stepSize;
    if (setR3Time < 0.0) setR3Time = 0.0;  // Min limit
    displaySetR3();
  } else if (buttonValue < buttonValues[4] && !buttonPressed) {  // Select button
    buttonPressed = true;
    // Save time to EEPROM
    EEPROM.put(EEPROM_ADDR_R3_TIME, setR3Time);
    lcd.clear();
    lcd.print("Successfully set");
    delay(2000);
    inSetR3Mode = false;
    inSettingsMode = true;
    displaySettingsMenu();
  }
}