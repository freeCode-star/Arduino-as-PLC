### User Manual & Flow Diagram for the Relay Control System

This is a user manual for the **Relay Control System** with LCD menu navigation and time-setting features. The system controls two relays based on user input, metal detection, and delay timings. It includes a main menu, settings for adjusting relay operation times, and start/stop modes for cycle execution.

---

### 1. **Overview**

The system is a user interface for controlling two relays based on timing and external conditions. The main components involved are:

- **LCD** for displaying information.
- **Two relays (Relay 1 & Relay 2)** for controlling external devices.
- **Metal sensor (PNP)** for detecting metal objects.
- **External button** to toggle between start and stop modes.
- **Buttons** for navigating menus and setting relay timings.

---

### 2. **System Features**
1. **Main Menu**  
   - Allows the user to navigate to settings or start the system.
   
2. **Settings Menu**  
   - In the settings, the user can adjust:
     - **Relay 1 time** (duration for which Relay 1 is active).
     - **Delay time** (delay between Relay 1 and Relay 2 activation).
   
3. **Start Mode**  
   - When metal is detected by the sensor, the system starts a cycle:
     - Relay 1 turns on for a user-set time.
     - Relay 2 turns on after a user-set delay.

4. **Cycle Restart**  
   - If metal is detected during the cycle, it restarts the entire cycle.

5. **Button Controls**  
   - External button toggles between **Start/Stop** mode.
   - Menu navigation buttons allow users to select options or adjust settings.

---

### 3. **Flow Diagram**

Here’s the flow of the system, including the main processes and conditions:

#### **Main Loop (Menu & Mode Navigation)**

```plaintext
[START]
    ↓
[Read Button Input] - Read values from button, external button, and sensor.
    ↓
[Button for External Mode] → [Start Mode?]
            ↘ Yes
                ↓
          [Enter Start Mode] → [Cycle Running?] → No → [Start Relay 1 & Delay Time]
                                         ↓
                                       Yes → [Relay 1 Time Over?]
                                         ↓
                                    Yes → [Start Relay 2 (After Delay)]
                                    ↘ No
                                       ↓
                               [Continue Checking Time]
                                       ↓
                        [Metal Detected?] → Yes → [Restart Cycle]
                                       ↓
[Main Menu] → [Settings Menu] → [Adjust Times for Relay 1 or Delay]
   ↓   ↓      ↓    ↓         ↑     ↑
[Back Button]  [Navigate Up] [Save & Return]  
```  

---

### 4. **Detailed Steps for Operation**

1. **Main Menu**:
   - Displays an option for entering the **Settings** menu.
   - Button presses allow navigation between options.
   - Selecting **Settings** enters the settings mode.

2. **Settings Mode**:
   - In **Settings**, the user can select:
     - **Set R1**: Adjust Relay 1 time.
     - **Set Delay Time**: Adjust the delay time for Relay 2.
     - **Back**: Return to the main menu.

3. **Setting Relay Times**:
   - **Set R1 Time**: Allows increment/decrement of Relay 1 time in 0.1-second steps.
   - **Set Delay Time**: Allows adjustment of the delay time between Relay 1 and Relay 2 activation.

4. **Start Mode**:
   - Metal detection starts the cycle.
   - Relay 1 is activated first for the set time.
   - Relay 2 is activated after the set delay.
   - If metal is detected during the cycle, the system restarts the cycle.

5. **Cycle Behavior**:
   - **Relay 1** is turned on for the specified time.
   - **Relay 2** is turned on after the delay.
   - If metal is detected, the cycle restarts from Relay 1.

6. **External Button Control**:
   - Pressing the external button toggles **Start/Stop Mode**.
   - In Start mode, the relays are activated according to the set times.
   - In Stop mode, both relays are turned off, and the system returns to the main menu.

---

### 5. **EEPROM Use**

The system saves the settings for:
- **Relay 1 time**: Stored in EEPROM address `0x00`.
- **Delay time**: Stored in EEPROM address `0x04`.

These values are loaded during startup, allowing the system to remember the last set values.

---

### 6. **User Interface:**

- **LCD Screen**: Displays current system state (e.g., "R1 ON", "R2 OFF", "Machine started", etc.).
- **Menu Navigation**: Use the buttons to navigate through menus and set values.
- **Button Presses**:
   - **Up/Down** to navigate.
   - **Select** to choose options.
   - **Back** to exit current mode/menu.

---

### 7. **Example Scenario**

- **Initial Setup**:
  - Upon power-up, the system loads the last saved values for Relay 1 time and Delay time from EEPROM.
  - The main menu shows: "1: Settings". Selecting it takes the user to the settings menu.

- **Setting Relay Times**:
  - In the settings menu, the user can choose "Set R1" to adjust the Relay 1 time, or "Set Delay Time" to adjust the delay time for Relay 2.
  - After adjusting, values are saved to EEPROM.

- **Start Mode**:
  - When metal is detected, Relay 1 is activated for the set time, followed by Relay 2 after the set delay.
  - If metal is detected again during the cycle, it restarts the cycle from Relay 1.

---

### 8. **Troubleshooting & Maintenance**

- **System Not Starting**: Ensure the external button is functioning and press it to toggle the system's start/stop mode.
- **Incorrect Time Values**: Check if the EEPROM stores the settings correctly. Adjust times using the settings menu.
- **Metal Detection Failure**: Verify that the PNP sensor is connected and operational.

---

This concludes the user manual for the Relay Control System.

In your code, both digital and analog pins are used for various purposes, such as controlling relays, buttons, and other external devices, as well as reading sensors. Below is a summary of the digital and analog pins used and their respective purposes:

### **Digital Pins (D0 to D10):**

1. **D0 (Pin 0)**:
   - **Purpose**: Detects short circuits (used to check if D0 is shorted to GND). 
   - **Functionality**: The state of this pin is checked to detect a short circuit event. When a short is detected, it toggles the state of A3.
   - **Pin Mode**: `INPUT_PULLUP`

2. **D1 (Pin 1)** (External Button Pin):
   - **Purpose**: Used as an external button input (button connected to this pin).
   - **Functionality**: When this button is pressed (shorted to GND), it triggers the start/stop of the cycle.
   - **Pin Mode**: `INPUT_PULLUP`

3. **D2 (Pin 2)** (Relay 1 Pin):
   - **Purpose**: Controls the first relay (Relay 1).
   - **Functionality**: This pin turns Relay 1 ON or OFF based on the cycle status.
   - **Pin Mode**: `OUTPUT`

4. **D3 (Pin 3)** (Relay 2 Pin):
   - **Purpose**: Controls the second relay (Relay 2).
   - **Functionality**: This pin turns Relay 2 ON or OFF after a delay (controlled by the timing logic).
   - **Pin Mode**: `OUTPUT`

5. **D4 (Pin 4)** to **D7 (Pin 7)** (LCD Pins):
   - **Purpose**: These pins are used for communication with the 16x2 LCD screen (using the 4-bit mode of the `LiquidCrystal` library).
   - **Functionality**: They handle the data and control lines for the LCD.
   - **Pin Modes**: `OUTPUT`
   - **Pins used**: 
     - **D4**: Data pin (4-bit mode)
     - **D5**: Data pin (4-bit mode)
     - **D6**: Data pin (4-bit mode)
     - **D7**: Data pin (4-bit mode)

6. **D8 (Pin 8)** (LCD RS Pin):
   - **Purpose**: Used to control the RS (Register Select) pin of the LCD.
   - **Functionality**: Used to select between command mode and data mode for the LCD.
   - **Pin Mode**: `OUTPUT`

7. **D9 (Pin 9)** (LCD Enable Pin):
   - **Purpose**: Controls the Enable pin for the LCD.
   - **Functionality**: This pin is used to enable communication between the Arduino and the LCD.
   - **Pin Mode**: `OUTPUT`

8. **D10 (Pin 10)** (LCD Backlight Pin):
   - **Purpose**: Controls the backlight of the LCD.
   - **Functionality**: This pin turns the backlight of the LCD ON or OFF.
   - **Pin Mode**: `OUTPUT`

---

### **Analog Pins (A0 to A3):**

1. **A0 (Pin A0)** (Button Input Pin):
   - **Purpose**: Reads the value from a potentiometer or button (depending on the configuration).
   - **Functionality**: This pin is used to read the analog value that is used to navigate the menu or adjust settings (like Relay timings).
   - **Pin Mode**: `INPUT`

2. **A1 (Pin A1)** (PNP Sensor Pin):
   - **Purpose**: Reads the state of a PNP sensor (used for metal detection).
   - **Functionality**: This pin is used to detect if metal is present. When the sensor is triggered, it indicates that metal is detected.
   - **Pin Mode**: `INPUT`

3. **A2 (Pin A2)** (External Device Control Pin):
   - **Purpose**: Controls an external device (for example, could be used to control a motor, actuator, etc.).
   - **Functionality**: This pin is used to turn an external device ON or OFF, depending on the system state (like in Start/Stop mode).
   - **Pin Mode**: `OUTPUT`
   - **Initial State**: `HIGH`

4. **A3 (Pin A3)** (Control Pin for Relay Logic):
   - **Purpose**: Toggles between HIGH and LOW states based on events such as a short circuit on D0.
   - **Functionality**: This pin is controlled to toggle its state when a short circuit is detected on D0. It is typically used for controlling external devices.
   - **Pin Mode**: `OUTPUT`

---

### **Summary Table:**

| Pin      | Purpose                                | Pin Mode        |
|----------|----------------------------------------|-----------------|
| **D0**   | Detects short circuit (input event)    | `INPUT_PULLUP`  |
| **D1**   | External button (Start/Stop control)   | `INPUT_PULLUP`  |
| **D2**   | Relay 1 control                        | `OUTPUT`        |
| **D3**   | Relay 2 control                        | `OUTPUT`        |
| **D4**   | LCD Data (4-bit mode)                  | `OUTPUT`        |
| **D5**   | LCD Data (4-bit mode)                  | `OUTPUT`        |
| **D6**   | LCD Data (4-bit mode)                  | `OUTPUT`        |
| **D7**   | LCD Data (4-bit mode)                  | `OUTPUT`        |
| **D8**   | LCD RS (Register Select)               | `OUTPUT`        |
| **D9**   | LCD Enable                             | `OUTPUT`        |
| **D10**  | LCD Backlight                          | `OUTPUT`        |
| **A0**   | Button value (analog input for menu)   | `INPUT`         |
| **A1**   | PNP Sensor (metal detection)           | `INPUT`         |
| **A2**   | External device control                | `OUTPUT`        |
| **A3**   | Control pin for event-based toggling   | `OUTPUT`        |

This table and the detailed explanations should give you a clear understanding of the role of each pin in your circuit.

To determine the "free" or unused pins in your current configuration, we will consider the pins that are not assigned any specific function in your code.

### Pins Already in Use:
Based on your previous explanation, here's the breakdown of used pins:

#### **Digital Pins (D0 to D10):**
- **D0**: Short circuit detection (`INPUT_PULLUP`)
- **D1**: Button input (`INPUT_PULLUP`)
- **D2**: Relay 1 control (`OUTPUT`)
- **D3**: Relay 2 control (`OUTPUT`)
- **D4** to **D7**: LCD data pins (`OUTPUT`)
- **D8**: LCD RS pin (`OUTPUT`)
- **D9**: LCD enable pin (`OUTPUT`)
- **D10**: LCD backlight pin (`OUTPUT`)

#### **Analog Pins (A0 to A3):**
- **A0**: Button (analog input) (`INPUT`)
- **A1**: PNP sensor (analog input) (`INPUT`)
- **A2**: External device control (`OUTPUT`)
- **A3**: Control pin for event toggling (`OUTPUT`)

---

### **Free Pins:**

#### **Digital Pins:**
- **D11**: Free
- **D12**: Free
- **D13**: Free

#### **Analog Pins:**
- **A4**: Free
- **A5**: Free

---

### **Summary:**
- **Free Digital Pins:** D11, D12, D13
- **Free Analog Pins:** A4, A5

These pins are not assigned in your current code and can be used for additional functions such as adding sensors, actuators, or other peripherals if needed.

Core Functionalities:

LCD Display:

The code initializes an LCD display to show menus and status information.
It uses the LiquidCrystal library to control the display.
Button Input:

The code reads input from a button connected to an analog pin.
It uses button press durations and thresholds to differentiate between single presses and long presses.
Relay Control:

The code controls two relays (RELAY1 and RELAY2) to switch on and off external devices.
It uses digitalWrite() to control the relay pins.
Timer-Based Operations:

The code uses millis() to measure time intervals and implement time-based actions.
It calculates elapsed time and remaining time for different operations.
EEPROM Storage:

The code stores and retrieves settings (R1 time and delay time) from EEPROM.
It uses the EEPROM library for data storage.
State Machine:

The code maintains different states (main menu, settings, R1 setting, R2 setting, start mode) and transitions between them based on button input.
It uses flags to track the current state and control the flow of execution.
Key Operations:

Main Menu:
Displays the main menu with options for "Settings" and "Start".
Navigates between menu items using the button.
Settings Menu:
Allows users to adjust R1 time and delay time.
Stores the settings in EEPROM.
Start Mode:
Starts and stops the machine cycle.
Controls the relays based on timers and metal detection.
Displays status information on the LCD.

Arduino as PLC our first Modal image

![IMG_20241207_173150](https://github.com/user-attachments/assets/99b66850-19f5-420b-a750-91ea3ef07f56)

![IMG_20241207_173205](https://github.com/user-attachments/assets/83ae4d3f-e9c1-4104-a83d-8362ac8bef66)

