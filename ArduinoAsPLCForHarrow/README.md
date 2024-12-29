# Arduino-Based PLC for Harrow Project

This project uses an **Arduino Nano** as a **Programmable Logic Controller (PLC)** to automate the control of relays for various components in a **harrow system**. The goal is to create a custom and reliable solution for controlling different devices and features with push buttons.

## Features

1. **Relay Control with Push Buttons:**
   - When **Button 1** is pressed, **Relay 1** is activated, and it turns off when the button is released.
   - When **Button 2** is pressed, **Relay 2** is activated, and it turns off when the button is released.

2. **Timed Relay Activation:**
   - When **Button 3** is pressed and released, **Relay 2** turns on for 10 seconds, and **Relay 3** turns on for 3 seconds. This cycle repeats each time the button is pressed.
   
3. **Timed Relay for Safety Feature:**
   - When **Button 4** is pressed and released, **Relay 3** turns on for 3 seconds to ensure safe operation.

## Purpose of the Project

This Arduino-based PLC aims to replace traditional PLC systems with a more cost-effective solution, leveraging the power of Arduino to control devices like relays, actuators, or sensors in a **harrow system** (or similar machinery).

The project is designed to be flexible and adaptable, allowing users to easily modify the relay logic, timing, and inputs to suit their needs.

## Components Used

- **Arduino Nano** (Main controller)
- **4 Push Buttons** (For user input)
- **3 Relay Modules** (For controlling external devices like motors, sensors, or actuators)
- **10kΩ Resistors** (For pull-down resistors on push buttons)
- **Jumper Wires** (For connecting components)
- **Power Supply** (For the Arduino and relays)

## Wiring Diagram

1. **Button 1:** Connect to Arduino Pin D2
2. **Button 2:** Connect to Arduino Pin D3
3. **Button 3:** Connect to Arduino Pin D4
4. **Button 4:** Connect to Arduino Pin D5
5. **Relay 1:** Connect to Arduino Pin D6
6. **Relay 2:** Connect to Arduino Pin D7
7. **Relay 3:** Connect to Arduino Pin D8

**Note:** Each button is connected to GND and uses the `INPUT_PULLUP` mode to simplify wiring (no external resistors required). The relay modules are connected to the Arduino's 5V and GND pins for power.

## Code Description

##Key Functions:

**Button Handling:** Each button controls one or more relays. The logic uses the internal pull-up resistor of the Arduino, so pressing a button connects the pin to GND, registering a LOW state.

**Relay Control:** Relays are turned ON or OFF based on button states or timed actions. Each relay has a dedicated pin on the Arduino to control its state.

**Timed Actions:** Buttons 3 and 4 trigger timed actions where specific relays are activated for a defined period (10 seconds for Relay 2 and 3 seconds for Relay 3).

# Installation Instructions

## Hardware Setup:

Connect the components as described in the Wiring Diagram section.

Ensure that the relays are connected to an appropriate external power supply if needed for the devices you're controlling.

Upload the provided Arduino code to the Arduino Nano using the Arduino IDE.

# Software Setup:

Open the Arduino IDE and create a new sketch.

Copy and paste the code provided above into the sketch.

Select the correct board and port from the Tools menu.

** Upload the code to your Arduino Nano. **
##Applications

This project can be used for various automation systems where you need to control multiple devices with simple push button interfaces, such as:

Agricultural equipment (like a harrow or other machinery)

Home automation systems

Industrial control systems

# Conclusion

This Arduino-based PLC solution is a flexible, cost-effective approach to controlling relays for various applications. It provides an excellent learning experience for anyone interested in automation systems and microcontroller programming.

Relay Control with Push Buttons:

When HydraulicButton is pressed, Relay 1 is activated, and it turns off when the button is released.
When ManualMotoronbutton is pressed, Motorrelay is activated, and it turns off when the button is released.
Timed Relay Activation:

When selector1 is pressed and released, Motorrelay turns on for 10 seconds, and BuzzerRelay turns on for 3 seconds. This cycle repeats each time the button is pressed.
Timed Relay for Safety Feature:

When selector2 is pressed and released, BuzzerRelay turns on for 3 seconds to ensure safe operation.
