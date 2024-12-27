#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>  // Include the EEPROM library

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Initialize the LCD

const int openSettingsPin = 9;  // Pin for the button to open settings
const int incrementPin = 10;    // Pin for the button to increment the blinking digit
const int shiftPin = 11;        // Pin for the button to move the blinking cursor to the next digit

int motorDurationArray[4] = {0, 0, 0, 0};  // Store the 4 digits of the motor duration
int buzzerDurationArray[4] = {0, 0, 0, 0}; // Store the 4 digits of the buzzer duration
int hydraDurationArray[4] = {0, 0, 0, 0};  // Store the 4 digits of the hydra duration
int pricePerHourArray[4] = {0, 0, 0, 0};   // Store the 4 digits of the price per hour
int gainFactorArray[4] = {0, 0, 0, 0};     // Store the 4 digits of the gain factor

int currentDigit = 0;  // Track the currently selected digit
unsigned long previousMillis = 0;  // To track time for blinking
const long blinkInterval = 500;  // Interval for blinking (in milliseconds)

bool inSettingsMode = false;  // Flag to track whether we're in settings mode
bool blinkState = false;  // State for blinking
bool dashboardMode = true;  // Track if we're in the dashboard mode
int settingIndex = 0;  // Track which setting we're editing (0 = motor, 1 = buzzer, 2 = hydra, 3 = price per hour, 4 = gain factor)

void setup() {
  pinMode(openSettingsPin, INPUT_PULLUP);  // Set Pin 9 as input with internal pull-up
  pinMode(incrementPin, INPUT_PULLUP);     // Set Pin 10 as input with internal pull-up
  pinMode(shiftPin, INPUT_PULLUP);         // Set Pin 11 as input with internal pull-up

  lcd.init();                      // Initialize the LCD
  lcd.backlight();                 // Turn on the LCD backlight

  // Initialize all EEPROM values to default (0000) if not set
  bool eepromInitialized = true;
  for (int i = 0; i < 20; i++) {
    if (EEPROM.read(i) != 0) {
      eepromInitialized = false;  // If any EEPROM value is not 0, it means the EEPROM is already initialized
      break;
    }
  }

  // If EEPROM is not initialized, set default values (0000) for all settings
  if (!eepromInitialized) {
    for (int i = 0; i < 4; i++) {
      motorDurationArray[i] = 0;  // Default to 0000
      EEPROM.write(i, 0);  // Save default values to EEPROM
    }
    for (int i = 4; i < 8; i++) {
      buzzerDurationArray[i - 4] = 0;  // Default to 0000
      EEPROM.write(i, 0);  // Save default values to EEPROM
    }
    for (int i = 8; i < 12; i++) {
      hydraDurationArray[i - 8] = 0;  // Default to 0000
      EEPROM.write(i, 0);  // Save default values to EEPROM
    }
    for (int i = 12; i < 16; i++) {
      pricePerHourArray[i - 12] = 0;  // Default to 0000
      EEPROM.write(i, 0);  // Save default values to EEPROM
    }
    for (int i = 16; i < 20; i++) {
      gainFactorArray[i - 16] = 0;  // Default to 0000
      EEPROM.write(i, 0);  // Save default values to EEPROM
    }
  } else {
    // Load settings from EEPROM if they are already set
    motorDurationArray[0] = EEPROM.read(0);
    motorDurationArray[1] = EEPROM.read(1);
    motorDurationArray[2] = EEPROM.read(2);
    motorDurationArray[3] = EEPROM.read(3);

    buzzerDurationArray[0] = EEPROM.read(4);
    buzzerDurationArray[1] = EEPROM.read(5);
    buzzerDurationArray[2] = EEPROM.read(6);
    buzzerDurationArray[3] = EEPROM.read(7);

    hydraDurationArray[0] = EEPROM.read(8);
    hydraDurationArray[1] = EEPROM.read(9);
    hydraDurationArray[2] = EEPROM.read(10);
    hydraDurationArray[3] = EEPROM.read(11);

    pricePerHourArray[0] = EEPROM.read(12);
    pricePerHourArray[1] = EEPROM.read(13);
    pricePerHourArray[2] = EEPROM.read(14);
    pricePerHourArray[3] = EEPROM.read(15);

    gainFactorArray[0] = EEPROM.read(16);
    gainFactorArray[1] = EEPROM.read(17);
    gainFactorArray[2] = EEPROM.read(18);
    gainFactorArray[3] = EEPROM.read(19);
  }

  // Display "Hi" initially
  lcd.setCursor(0, 0);
  lcd.print("Hi");
  lcd.setCursor(0, 1);
  lcd.print("        ");
}

void loop() {
  // If Pin 9 is shorted to GND, toggle between dashboard and settings mode
  if (digitalRead(openSettingsPin) == LOW) {
    delay(500);  // Debounce delay to avoid repeated triggering
    if (dashboardMode) {
      dashboardMode = false;  // Enter settings mode
      inSettingsMode = true;  // Enable settings mode
      settingIndex = 0;       // Start from the beginning of the settings (Motor Duration)
      lcd.clear();
    } else {
      dashboardMode = true;  // Enter dashboard mode
      inSettingsMode = false;  // Disable settings mode
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hi");
      lcd.setCursor(0, 1);
      lcd.print("        ");
    }
  }

  if (dashboardMode) {
    // In dashboard mode, just show "Hi"
    return;  // Do nothing, just wait for user input
  }

  // If we're in settings mode, handle all settings
  unsigned long currentMillis = millis();
  
  if (inSettingsMode) {
    // Display current setting based on settingIndex
    if (settingIndex == 0) {
      lcd.setCursor(0, 0);
      lcd.print("Set M Time");

      handleSetting(motorDurationArray);  // Handle motor duration setting
    } 
    else if (settingIndex == 1) {
      lcd.setCursor(0, 0);
      lcd.print("Set BUZZ Time");

      handleSetting(buzzerDurationArray);  // Handle buzzer duration setting
    }
    else if (settingIndex == 2) {
      lcd.setCursor(0, 0);
      lcd.print("Set Hydra Time");

      handleSetting(hydraDurationArray);  // Handle hydra duration setting
    }
    else if (settingIndex == 3) {
      lcd.setCursor(0, 0);
      lcd.print("Set Price/Hour");

      handleSetting(pricePerHourArray);  // Handle price per hour setting
    }
    else if (settingIndex == 4) {
      lcd.setCursor(0, 0);
      lcd.print("Set Gain Factor");

      handleSetting(gainFactorArray);  // Handle gain factor setting
    }
  }
}

void handleSetting(int settingArray[]) {
  unsigned long currentMillis = millis();

  // Check if it's time to blink the digit
  if (currentMillis - previousMillis >= blinkInterval) {
    previousMillis = currentMillis;  // Update the time for next blink
    blinkState = !blinkState;  // Toggle the blink state

    // Print the current setting values
    for (int i = 0; i < 4; i++) {
      lcd.setCursor(i, 1);  // Set the cursor for each digit
      if (i == currentDigit) {  // Only the selected digit should blink
        if (blinkState) {
          lcd.print(" ");  // Blank the current digit (blink effect)
        } else {
          lcd.print(settingArray[i]);  // Reprint the current digit
        }
      } else {
        lcd.print(settingArray[i]);  // Print the other digits normally
      }
    }
  }

  // If Pin 10 is shorted to GND, increment the blinking digit
  if (digitalRead(incrementPin) == LOW) {
    delay(500);  // Debounce delay to avoid repeated triggering
    settingArray[currentDigit]++;  // Increment the currently selected digit
    if (settingArray[currentDigit] > 9) {  // If the digit exceeds 9, reset to 0
      settingArray[currentDigit] = 0;
    }
  }

  // If Pin 11 is shorted to GND, move the blinking cursor to the next digit
  if (digitalRead(shiftPin) == LOW) {
    delay(500);  // Debounce delay to avoid repeated triggering
    currentDigit++;  // Move to the next digit
    if (currentDigit > 3) {  // If we've reached the last digit, go back to the first digit
      currentDigit = 0;
    }
  }

  // Once the user finishes setting, save it to EEPROM
  if (digitalRead(openSettingsPin) == LOW) {
    // Save the setting to EEPROM
    int startAddress = settingIndex * 4;
    for (int i = 0; i < 4; i++) {
      EEPROM.write(startAddress + i, settingArray[i]);
    }

    settingIndex++;  // Move to the next setting
    if (settingIndex > 4) {
      settingIndex = 0;  // After Gain Factor, return to Motor Duration
      dashboardMode = true;  // After Gain Factor, go back to dashboard mode
      inSettingsMode = false;  // Exit settings mode
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hi");
      lcd.setCursor(0, 1);
      lcd.print("        ");
    }
    delay(500);  // Debounce delay
  }
}
