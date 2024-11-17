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
const int BUTTON_PIN = A0; // Button connected to A0 pin

// Relay pin definitions
const int RELAY1_PIN = 2;
const int RELAY2_PIN = 3;

// Initialize the LCD with the interface pins
LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// Button value thresholds
int buttonValues[5] = {50, 200, 400, 600, 800}; 
int currentSelection = 0; 
bool inSettingsMode = false; 
bool inMainMenu = true; 
bool inSetR1Mode = false; 
bool inSetR2Mode = false; 
bool inStartMode = false;
bool buttonPressed = false;
float setR1Time = 0.0; // Time for Relay 1
float setR2Time = 0.0; // Time for Relay 2 delay
float stepSize = 0.1; // Increment/Decrement step size
unsigned long relay1StartTime = 0;
unsigned long relay2StartTime = 0;
bool isRelay1Active = false; // Track if Relay 1 is active
bool isRelay2Active = false; // Track if Relay 2 is active

// EEPROM addresses for saving times
const int EEPROM_ADDR_R1_TIME = 0;
const int EEPROM_ADDR_R2_TIME = 4; // Different EEPROM address for Set R2

// Menu items
char* menuItems[4] = {"1: Settings", "2: Manual Start", "3: Start", "4: Exit"}; 
char* settingItems[3] = {"Set R1", "Set R2", "Back"}; 
int currentSettingSelection = 0; 

void setup() {
    lcd.begin(16, 2);
    pinMode(BACKLIGHT, OUTPUT);
    digitalWrite(BACKLIGHT, HIGH);
    
    pinMode(RELAY1_PIN, OUTPUT);
    pinMode(RELAY2_PIN, OUTPUT);
    
    digitalWrite(RELAY1_PIN, LOW);
    digitalWrite(RELAY2_PIN, LOW);
    
    // Load saved times from EEPROM
    EEPROM.get(EEPROM_ADDR_R1_TIME, setR1Time);
    EEPROM.get(EEPROM_ADDR_R2_TIME, setR2Time);
    
    // If R1 or R2 time are uninitialized, assign default values
    if (isnan(setR1Time) || setR1Time < 0.0 || setR1Time > 99.9) {
        setR1Time = 0.0;
    }
    if (isnan(setR2Time) || setR2Time < 0.0 || setR2Time > 99.9) {
        setR2Time = 0.0;
    }
    
    displayMenu();
}

void loop() {
    int buttonValue = analogRead(BUTTON_PIN);

    if (inMainMenu) {
        handleMainMenu(buttonValue);
    } else if (inSettingsMode) {
        handleSettingsMode(buttonValue);
    } else if (inSetR1Mode) {
        handleSetR1(buttonValue);
    } else if (inSetR2Mode) {
        handleSetR2(buttonValue);
    } else if (inStartMode) {
        handleStartMode();
    }

    // Handle button press to navigate back
    if (buttonValue < buttonValues[0] && !buttonPressed) { // Right button
        buttonPressed = true;
        if (inSetR1Mode) {
            inSetR1Mode = false;
            inSettingsMode = true;
            displaySettingsMenu();
        } else if (inSetR2Mode) {
            inSetR2Mode = false;
            inSettingsMode = true;
            displaySettingsMenu();
        } else if (inStartMode) {
            // Turn off both relays
            digitalWrite(RELAY1_PIN, LOW);
            digitalWrite(RELAY2_PIN, LOW);
            lcd.clear();
            lcd.print("Relays OFF");
            delay(2000);
            inStartMode = false; // Return to dashboard
            displayMenu();
        }
    }

    if (buttonValue >= buttonValues[0]) {
        buttonPressed = false;
    }

    delay(200);
}

void handleMainMenu(int buttonValue) {
    if (buttonValue < buttonValues[1] && !buttonPressed) { 
        if (currentSelection > 0) {
            currentSelection--;
            displayMenu();
        }
    } else if (buttonValue < buttonValues[2] && !buttonPressed) {
        if (currentSelection < 3) {
            currentSelection++;
            displayMenu();
        }
    } else if (buttonValue < buttonValues[4] && !buttonPressed) {
        buttonPressed = true;
        if (currentSelection == 0) {
            enterSettingsMode();
        } else if (currentSelection == 2) { // Start mode
            inStartMode = true;
            relay1StartTime = millis(); // Start the timer for R1
            digitalWrite(RELAY1_PIN, HIGH); // Ensure relay 1 is on immediately
            isRelay1Active = true; // Track that R1 is active
            displayStartMode(); // Prepare for display
        }
    }
}

void displayMenu() {
    lcd.clear();
    for (int i = 0; i < 2; i++) {
        lcd.setCursor(1, i);
        lcd.print(menuItems[currentSelection + i]);
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

        if (currentSettingSelection == 2) { 
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
        }
    }
}

void displaySettingsMenu() {
    lcd.clear();
    for (int i = 0; i < 2; i++) {
        lcd.setCursor(1, i);
        if (currentSettingSelection + i < 3) {
            lcd.print(settingItems[currentSettingSelection + i]);
        }
    }
    
    lcd.setCursor(0, 0);
    lcd.write('>');
}

void displaySetR1() {
    lcd.clear();
    lcd.print("Set R1 Time:");
    lcd.setCursor(0, 1);
    lcd.print(setR1Time, 1); // Display time with one decimal place
    lcd.print(" sec");
}

void handleSetR1(int buttonValue) {
    if (buttonValue < buttonValues[1] && !buttonPressed) { // Up button
        setR1Time += stepSize;
        if (setR1Time > 99.9) setR1Time = 99.9; // Max limit
        displaySetR1();
    } else if (buttonValue < buttonValues[2] && !buttonPressed) { // Down button
        setR1Time -= stepSize;
        if (setR1Time < 0.0) setR1Time = 0.0; // Min limit
        displaySetR1();
    } else if (buttonValue < buttonValues[4] && !buttonPressed) { // Select button
        buttonPressed = true;
        // Save time to EEPROM
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
    lcd.print("Set R2 Delay:");
    lcd.setCursor(0, 1);
    lcd.print(setR2Time, 1); // Display time with one decimal place
    lcd.print(" sec");
}

void handleSetR2(int buttonValue) {
    if (buttonValue < buttonValues[1] && !buttonPressed) { // Up button
        setR2Time += stepSize;
        if (setR2Time > 99.9) setR2Time = 99.9; // Max limit
        displaySetR2();
    } else if (buttonValue < buttonValues[2] && !buttonPressed) { // Down button
        setR2Time -= stepSize;
        if (setR2Time < 0.0) setR2Time = 0.0; // Min limit
        displaySetR2();
    } else if (buttonValue < buttonValues[4] && !buttonPressed) { // Select button
        buttonPressed = true;
        EEPROM.put(EEPROM_ADDR_R2_TIME, setR2Time);
        lcd.clear();
        lcd.print("Successfully set");
        delay(2000); 
        inSetR2Mode = false;
        inSettingsMode = true;
        displaySettingsMenu();
    }
}

void displayStartMode() {
    lcd.clear();
    lcd.print("Relay 1 countdown:");
    lcd.setCursor(0, 1);
    lcd.print(setR1Time, 1);
    lcd.print(" sec");
}

void handleStartMode() {
    unsigned long currentTime = millis();
    if (isRelay1Active) {
        float elapsedTime = (currentTime - relay1StartTime) / 1000.0;
        float remainingTime = setR1Time - elapsedTime;
        lcd.setCursor(0, 1);
        lcd.print(remainingTime, 1);
        lcd.print(" sec  ");
        if (remainingTime <= 0) {
            isRelay1Active = false;
            digitalWrite(RELAY1_PIN, LOW);
            relay2StartTime = millis();
            isRelay2Active = true; 
            displayRelay2Countdown();
        }
    } else if (isRelay2Active) {
        float elapsedTime = (currentTime - relay2StartTime) / 1000.0;
        float remainingTime = setR2Time - elapsedTime;
        lcd.setCursor(0, 1);
        lcd.print(remainingTime, 1);
        lcd.print(" sec  ");
        if (remainingTime <= 0) {
            isRelay2Active = false;
            digitalWrite(RELAY2_PIN, HIGH);
            relay2StartTime = millis(); // Store start time of relay 2 operation
            displayRelay2Operation();
        }
    } else {
        float relay2Elapsed = (currentTime - relay2StartTime) / 1000.0;
        float relay2Remaining = setR1Time - relay2Elapsed;
        lcd.setCursor(0, 1);
        lcd.print(relay2Remaining, 1);
        lcd.print(" sec  ");
        if (relay2Remaining <= 0) {
            isRelay2Active = false;
            digitalWrite(RELAY2_PIN, LOW);
        }
    }
}

void displayRelay2Countdown() {
    lcd.clear();
    lcd.print("Relay 2 delay:");
    lcd.setCursor(0, 1);
    lcd.print(setR2Time, 1);
    lcd.print(" sec");
}

void displayRelay2Operation() {
    lcd.clear();
    lcd.print("Relay 2 countdown:");
    lcd.setCursor(0, 1);
    lcd.print(setR1Time, 1);
    lcd.print(" sec");
}
