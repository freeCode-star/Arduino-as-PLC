const int selector1Pin = 2;   // Pin where Button 1 is connected
const int motorPin = 3;   // Pin where Relay 1 is connected
const int buzzerPin = 4;   // Pin where Relay 3 is connected
const int manualMotorOnPin = 5;  // Pin where Button 2 is connected
const int selector2Pin = 6;  // Pin where Button 3 is connected (new button)
const int hydraulicButtonPin = 7;  // Pin where Button 4 is connected (new button)
const int hydraulicRelayPin = 8;   // Pin where Relay 4 is connected (new relay)

unsigned long motorOnTime = 0;   // Time when Relay 1 was turned on
unsigned long buzzerOnTime = 0;   // Time when Relay 3 was turned on
unsigned long motorDuration = 10000;  // Relay 1 on for 10 seconds
unsigned long buzzerDuration = 3000;   // Relay 3 on for 3 seconds
unsigned long relay1ShortDuration = 3000;  // Relay 1 for 3 seconds (new logic)

bool motorState = false;   // To track the state of Relay 1
bool buzzerState = false;   // To track the state of Relay 3
bool buzzerOnState = false; // To track Relay 1's short state
bool relay4State = false;   // To track the state of Relay 4

bool lastselector1State = LOW;  // Previous state of Button 1
bool selector1State = LOW;      // Current state of Button 1
bool manualMotorOn = LOW;     // Current state of Button 2
bool selector2State = LOW;     // Current state of Button 3
bool hydraulicState = LOW;     // Current state of Button 4

void setup() {
  pinMode(selector1Pin, INPUT_PULLUP);  // Set Button 1 pin as input with pull-up resistor
  pinMode(manualMotorOnPin, INPUT_PULLUP);  // Set Button 2 pin as input with pull-up resistor
  pinMode(selector2Pin, INPUT_PULLUP);  // Set Button 3 pin as input with pull-up resistor
  pinMode(hydraulicButtonPin, INPUT_PULLUP);  // Set Button 4 pin as input with pull-up resistor

  pinMode(motorPin, OUTPUT);        // Set Relay 1 pin as output
  pinMode(buzzerPin, OUTPUT);        // Set Relay 3 pin as output
  pinMode(hydraulicRelayPin, OUTPUT);        // Set Relay 4 pin as output
  digitalWrite(motorPin, LOW);      // Ensure Relay 1 is off initially
  digitalWrite(buzzerPin, LOW);      // Ensure Relay 3 is off initially
  digitalWrite(hydraulicRelayPin, LOW);      // Ensure Relay 4 is off initially
}

void loop() {
  // Read the current button states
  selector1State = digitalRead(selector1Pin);  // Button 1 state
  manualMotorOn = digitalRead(manualMotorOnPin);  // Button 2 state
  selector2State = digitalRead(selector2Pin);  // Button 3 state
  hydraulicState = digitalRead(hydraulicButtonPin);  // Button 4 state

  // Button 1 pressed: turn both relays on for specified durations
  if (selector1State == LOW && lastselector1State == HIGH) {
    // Button 1 was pressed, turn on both relays
    motorState = true;
    buzzerState = true;
    motorOnTime = millis();  // Store the time when Relay 1 was turned on
    buzzerOnTime = millis();  // Store the time when Relay 3 was turned on
    digitalWrite(motorPin, HIGH);  // Turn Relay 1 on
    digitalWrite(buzzerPin, HIGH);  // Turn Relay 3 on
  }

  // Check if Relay 1 has been on for 10 seconds
  if (motorState && (millis() - motorOnTime >= motorDuration)) {
    digitalWrite(motorPin, LOW);  // Turn Relay 1 off
    motorState = false;  // Reset Relay 1 state
  }

  // Check if Relay 3 has been on for 3 seconds (from Button 1)
  if (buzzerState && (millis() - buzzerOnTime >= buzzerDuration)) {
    digitalWrite(buzzerPin, LOW);  // Turn Relay 3 off
    buzzerState = false;  // Reset Relay 3 state
  }

  // Button 2 pressed: control Relay 3 independently
  if (manualMotorOn == LOW) {
    digitalWrite(motorPin, HIGH);  // Keep Relay 3 on while Button 2 is pressed
  } else {
    // Only turn off Relay 3 when Button 2 is released
    if (millis() - motorOnTime >= motorDuration) {
      digitalWrite(motorPin, LOW);   // Turn Relay 3 off when Button 2 is not pressed
    }
  }

  // Button 3 pressed: turn Relay 1 on for 3 seconds
  if (selector2State == LOW && buzzerOnState == false) {
    // Button 3 was pressed, turn Relay 1 on for 3 seconds
    buzzerOnState = true;   // Set short duration state for Relay 1
    digitalWrite(buzzerPin, HIGH);  // Turn Relay 1 on
    motorOnTime = millis();  // Start the timer for 3 seconds
  }

  // Check if Relay 1 has been on for 3 seconds (from Button 3)
  if (buzzerOnState && (millis() - motorOnTime >= relay1ShortDuration)) {
    digitalWrite(buzzerPin, LOW);  // Turn Relay 1 off after 3 seconds
    buzzerOnState = false;  // Reset Relay 1 short state
  }

  // Button 4 pressed: control Relay 4 (new button)
  if (hydraulicState == LOW) {
    // Button 4 is pressed, turn Relay 4 on
    digitalWrite(hydraulicRelayPin, HIGH);  // Turn Relay 4 on
  } else {
    // Button 4 is released, turn Relay 4 off
    digitalWrite(hydraulicRelayPin, LOW);   // Turn Relay 4 off
  }

  // Update the last button state for Button 1
  lastselector1State = selector1State;
}