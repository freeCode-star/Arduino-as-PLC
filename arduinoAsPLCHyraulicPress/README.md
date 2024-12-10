Arduino as PLC for Hydraulic Press Control - Open Source Relay Control System
This project demonstrates how to use an Arduino as a PLC (Programmable Logic Controller) for controlling a hydraulic press, with advanced features like metal detection, timing control, and cycle restart. It is designed to offer a reliable and cost-effective alternative to traditional PLC systems in industrial applications. All code and schematics are open source and free to use.

Overview
This system allows you to control a hydraulic press or any other industrial machinery using an Arduino, acting as a PLC, with real-time control over relays, metal detection, and cycle timing. The setup includes:

Relay Control: Two relays control external devices like solenoids, actuators, or pumps.
Metal Detection: A PNP sensor detects metal objects to trigger actions.
Cycle Control: Adjustable start/stop cycle, timing for relays, and delay for sequential operations.
LCD Menu Interface: Navigate menus to adjust settings like relay timings and system operation.
Key Features
Industrial-Grade Relay Control:
Control up to two relays to manage industrial actuators or systems like a hydraulic press.

Flexible Timing:
Adjust the activation time of Relay 1 and the delay before Relay 2 is activated. Perfect for sequential processes in manufacturing or automation.

Cycle Restart:
The system automatically restarts the cycle if metal is detected during operations, ensuring continuous automation.

User Interface:
Navigate settings via an intuitive LCD menu, with options to configure timing, start/stop cycles, and control system operations.

External Button Control:
Toggle between Start/Stop modes using an external button for manual control of the system.

System Components
Arduino (Uno or Nano): Acts as the PLC to control operations.
LCD Screen: For displaying menu options and system status.
Two Relays (Relay 1 & Relay 2): Control external equipment like motors or solenoids.
PNP Metal Sensor: Detects the presence of metal objects to trigger system actions.
External Button: Used to start and stop the system manually.
How It Works
Menu & Navigation
The system includes two main modes:

Main Menu: Navigate to start the cycle or enter settings mode.
Settings Mode: Adjust Relay 1 time and Delay time settings.
Once metal is detected, the cycle begins:

Relay 1 activates for the specified time.
After a set delay, Relay 2 activates.
If metal is detected during the cycle, the process restarts.
Flow Diagram
The flow diagram below illustrates the core functionality:

[START] --> [Read Button & Sensor Inputs] --> [Check Mode] --> [Start Cycle or Menu]
    ↓                            ↓                      ↓
[Metal Detected?] --> Yes --> [Start Relay 1] --> [Delay] --> [Start Relay 2]
    ↓                        ↓
[Restart Cycle if Metal is Detected]

System Setup
Main Menu:
Displays the option to enter Settings or begin the cycle.

Settings Mode:

Adjust Relay 1 time (in seconds).
Adjust Delay Time (between Relay 1 and Relay 2).
Save settings to EEPROM for persistence.
Cycle Operation:
When metal is detected:

Relay 1 activates for the set time.
Relay 2 follows after the user-defined delay.
If metal is detected again, the system restarts from Relay 1.
Button Controls:

External Button toggles between Start/Stop modes.
Use Up/Down buttons for navigating settings.
Pin Configuration
Digital Pins (D0 to D10):
D0: Short circuit detection (input)
D1: External button (input)
D2: Relay 1 control (output)
D3: Relay 2 control (output)
D4-D7: LCD communication (outputs)
D8-D10: LCD control pins (outputs)
Analog Pins (A0 to A3):
A0: Button input (analog navigation)
A1: PNP sensor input (metal detection)
A2: External device control (output)
A3: Control pin for event toggling (output)
Example Workflow
Power-Up:
The system loads saved settings (Relay 1 time and delay) from EEPROM.

Settings:
The user can adjust timing values for Relay 1 and Delay in the settings menu.

Cycle Execution:
Metal triggers the start of Relay 1. After the specified delay, Relay 2 activates. The cycle restarts if metal is detected mid-cycle.

EEPROM Storage
Relay 1 Time: Stored in EEPROM at address 0x00.
Delay Time: Stored in EEPROM at address 0x04.
These values are loaded automatically when the system starts, providing persistence between restarts.

Troubleshooting
System Not Starting:
Ensure the external button is correctly wired and functioning to toggle start/stop mode.

Incorrect Time Settings:
Verify EEPROM data is correctly stored. If settings are not saving, check the EEPROM write process.

Metal Detection Issues:
Ensure the PNP sensor is properly connected and calibrated for accurate metal detection.

Contribute & License
This project is open source under the MIT License. Contributions are welcome! Feel free to fork and modify this repository for your industrial applications.

Demo Image

![IMG_20241207_173150](https://github.com/user-attachments/assets/99b66850-19f5-420b-a750-91ea3ef07f56)

![IMG_20241207_173205](https://github.com/user-attachments/assets/83ae4d3f-e9c1-4104-a83d-8362ac8bef66)

Conclusion
This Arduino-based PLC is a cost-effective, open-source solution for controlling hydraulic presses and other industrial machinery. With adjustable timing, metal detection, and customizable cycles, this system can be adapted to a wide variety of industrial automation tasks.
