// const int selector1Pin = 2;   // Pin where Selector 1 is connected
// const int selector2Pin = 3;   // Pin where Selector 2 is connected (new button)
// const int manualMotorOnPin = 4;  // Pin where Manual Motor Button is connected
// const int hydraulicButtonPin = 5;  // Pin where Hydraulic On Button is connected (new button)
// const int hydraulicRelayPin = 6;   // Pin where Hydraulic Relay is connected (new relay)
// const int motorRelayPin = 7;   // Pin where Motor Relay is connected
// const int buzzerRelayPin = 8;   // Pin where Buzzer Relay is connected

// const int autoManualButtonPin = 9;  // Pin where the Automanual Toggle Button is connected

// unsigned long motorOnTime = 0;   // Time when Motor Relay was turned on
// unsigned long buzzerOnTime = 0;   // Time when Buzzer Relay was turned on
// unsigned long motorDuration = 10000;  // Motor Relay on for 10 seconds
// unsigned long buzzerDuration = 3000;   // Buzzer Relay on for 3 seconds
// unsigned long relay1ShortDuration = 3000;  // Motor Relay for 3 seconds (new logic)

// bool motorState = false;   // To track the state of Motor Relay
// bool buzzerState = false;   // To track the state of Buzzer Relay
// bool buzzerOnState = false; // To track Motor Relay's short state
// bool relay4State = false;   // To track the state of Hydraulic Relay

// bool lastselector1State = LOW;  // Previous state of Selector 1
// bool selector1State = LOW;      // Current state of Selector 1
// bool manualMotorOn = LOW;     // Current state of Manual Motor Button
// bool selector2State = LOW;     // Current state of Selector 2
// bool hydraulicState = LOW;     // Current state of Hydraulic On Button
// bool autoManualState = false;  // State of Automanual Mode (false = Manual, true = Auto)

// void setup() {
//   pinMode(selector1Pin, INPUT_PULLUP);  // Set Selector 1 pin as input with pull-up resistor
//   pinMode(manualMotorOnPin, INPUT_PULLUP);  // Set Manual Motor Button pin as input with pull-up resistor
//   pinMode(selector2Pin, INPUT_PULLUP);  // Set Selector 2 pin as input with pull-up resistor
//   pinMode(hydraulicButtonPin, INPUT_PULLUP);  // Set Hydraulic On Button pin as input with pull-up resistor
//   pinMode(autoManualButtonPin, INPUT_PULLUP);  // Set Automanual Toggle Button pin as input with pull-up resistor

//   pinMode(motorRelayPin, OUTPUT);        // Set Motor Relay pin as output
//   pinMode(buzzerRelayPin, OUTPUT);        // Set Buzzer Relay pin as output
//   pinMode(hydraulicRelayPin, OUTPUT);        // Set Hydraulic Relay pin as output
//   digitalWrite(motorRelayPin, HIGH);      // Ensure Motor Relay is off initially
//   digitalWrite(buzzerRelayPin, HIGH);      // Ensure Buzzer Relay is off initially
//   digitalWrite(hydraulicRelayPin, HIGH);      // Ensure Hydraulic Relay is off initially
// }

// void loop() {
//   // Read the current button states
//   selector1State = digitalRead(selector1Pin);  // Selector 1 state
//   manualMotorOn = digitalRead(manualMotorOnPin);  // Manual Motor Button state
//   selector2State = digitalRead(selector2Pin);  // Selector 2 state
//   hydraulicState = digitalRead(hydraulicButtonPin);  // Hydraulic On Button state
//   bool autoManualButtonState = digitalRead(autoManualButtonPin);  // Automanual Button state

//   // Toggle Automanual mode when the button is pressed
//   if (autoManualButtonState == LOW) {
//     autoManualState = !autoManualState;  // Toggle Auto/Manual mode
//     delay(300);  // Debounce delay for button press
//   }

//   // If in Auto mode, do not allow Manual Motor Button to work
//   if (autoManualState) {
//     // Auto mode logic (your existing auto behavior for motor, hydraulic, etc.)
//     // Example: Hydraulic control remains the same in Auto mode
//     if (hydraulicState == LOW) {
//       digitalWrite(hydraulicRelayPin, LOW);  // Turn Hydraulic Relay on
//     } else {
//       digitalWrite(hydraulicRelayPin, HIGH);   // Turn Hydraulic Relay off
//     }
//   } 
//   else {
//     // Manual mode logic: Only manual motor on works when in Manual mode
//     if (manualMotorOn == LOW) {
//       digitalWrite(motorRelayPin, LOW);  // Keep Motor Relay on while Manual Motor Button is pressed
//     } else {
//       // Only turn off Motor Relay when Manual Motor Button is released
//       if (millis() - motorOnTime >= motorDuration) {
//         digitalWrite(motorRelayPin, HIGH);   // Turn Motor Relay off
//       }
//     }
//   }

//   // Selector 1 pressed: turn both relays on for specified durations
//   if (selector1State == LOW && lastselector1State == HIGH) {
//     motorState = true;
//     buzzerState = true;
//     motorOnTime = millis();  // Store the time when Motor Relay was turned on
//     buzzerOnTime = millis();  // Store the time when Buzzer Relay was turned on
//     digitalWrite(motorRelayPin, LOW);  // Turn Motor Relay on
//     digitalWrite(buzzerRelayPin, LOW);  // Turn Buzzer Relay on
//   }

//   // Check if Motor Relay has been on for 10 seconds
//   if (motorState && (millis() - motorOnTime >= motorDuration)) {
//     digitalWrite(motorRelayPin, HIGH);  // Turn Motor Relay off
//     motorState = false;  // Reset Motor Relay state
//   }

//   // Check if Buzzer Relay has been on for 3 seconds (from Selector 1)
//   if (buzzerState && (millis() - buzzerOnTime >= buzzerDuration)) {
//     digitalWrite(buzzerRelayPin, HIGH);  // Turn Buzzer Relay off
//     buzzerState = false;  // Reset Buzzer Relay state
//   }

//   // Selector 2 pressed: turn Motor Relay on for 3 seconds
//   if (selector2State == LOW && buzzerOnState == false) {
//     buzzerOnState = true;   // Set short duration state for Motor Relay
//     digitalWrite(buzzerRelayPin, LOW);  // Turn Motor Relay on
//     motorOnTime = millis();  // Start the timer for 3 seconds
//   }

//   // Check if Motor Relay has been on for 3 seconds (from Selector 2)
//   if (buzzerOnState && (millis() - motorOnTime >= relay1ShortDuration)) {
//     digitalWrite(buzzerRelayPin, HIGH);  // Turn Motor Relay off after 3 seconds
//     buzzerOnState = false;  // Reset Motor Relay short state
//   }

//   // Hydraulic On Button pressed: control Hydraulic Relay (new button)
//   if (hydraulicState == LOW) {
//     digitalWrite(hydraulicRelayPin, LOW);  // Turn Hydraulic Relay on
//   } else {
//     digitalWrite(hydraulicRelayPin, HIGH);   // Turn Hydraulic Relay off
//   }

//   // Update the last button state for Selector 1
//   lastselector1State = selector1State;
// }

#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;  // Create an RTC object

void setup() {
  Serial.begin(9600);  // Start serial communication

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);  // Halt if RTC is not found
  }

  // Check if the RTC has lost power
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");

    // Set the current time. Modify this line to set your specific time:
    rtc.adjust(DateTime(2024, 12, 24, 10, 0, 0));  // Set to Dec 24, 2024, 10:00:00
    // Alternatively, you can use __DATE__ and __TIME__ for compile-time settings:
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Print the current time after setting
  printTime();
}

void loop() {
  // Print the time every 1 second
  printTime();
  delay(1000);
}

void printTime() {
  DateTime now = rtc.now();  // Get the current time from RTC

  // Display the time in the serial monitor
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
}
