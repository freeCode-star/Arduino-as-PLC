// Define the pin where the relay is connected
int relayPin = 13;

void setup() {
  // Initialize pin 13 as an OUTPUT
  pinMode(relayPin, OUTPUT);
}

void loop() {
  // Turn on the relay (HIGH means ON)
  digitalWrite(relayPin, LOW);
  delay(1000);  // Wait for 1 second (1000 milliseconds)
  
  // Turn off the relay (LOW means OFF)
  digitalWrite(relayPin, HIGH);
  delay(1000);  // Wait for 1 second
}
