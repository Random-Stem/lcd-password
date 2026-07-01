# Arduino LCD Password System & RFID Access Control

This project consists of a dual-Arduino system that implements an interactive security infrastructure. **Arduino 1** manages a password-protected interface with built-in mini-games on an LCD screen. **Arduino 2** handles a contactless security layer using an MFRC522 RFID reader to validate unique card IDs and trigger physical outputs.

---

## 📌 System 1: LCD Password & Mini-Games (Main Arduino)

This board manages user access via a 4x4 matrix keypad, handles an emergency fallback menu, and runs an embedded game to interact with the user on an LCD 16x2 screen.

### Key Features
*   **Access Control:** Verifies a user-defined password (default: `1111`) masked with asterisks `*` on the display.
*   **SOS Emergency Menu:** Entering the override code `0000` opens a restricted menu to reset the password.
*   **RFID Authorization Trigger:** Receives an external high signal to authorize password modifications.
*   **Integrated Dino Game:** Includes a custom rendering engine using Arduino RAM-stored character bytes to display a jumping dinosaur and oncoming obstacles, tracking your live score.

### Hardware Wiring Configuration
Connect your components to the first Arduino board using the following pin map:

#### 1. LCD Display 16x2 (I2C Module)
*   **VCC** -> 5V
*   **GND** -> GND
*   **SDA** -> Arduino SDA Pin (Pin A4 on Arduino Uno)
*   **SCL** -> Arduino SCL Pin (Pin A5 on Arduino Uno)
*   *Note: Default I2C hardware address is configured to `0x3F`.*

#### 2. 4x4 Matrix Keypad
*   **Rows (Pins 1-4 on Keypad)** -> Digital Pins **2, 3, 4, 5**
*   **Columns (Pins 5-8 on Keypad)** -> Digital Pins **6, 7, 8, 9**

#### 3. Analog Controls & Navigation Buttons
*   **Pin A0:** Analog input linked to a button network/potentiometer to navigate Up/Down through menus.
*   **Pin A1:** Generic analog reading pin.
*   **Pin A2:** Selection/Confirmation button (configured as `INPUT_PULLUP`, triggers when pulled to GND).

#### 4. Outputs & Game Interactivity
*   **Pin 10 (Output):** Connected to a piezo **Buzzer** for sound effects (correct/wrong key tones).
*   **Pin 11 (Input Pullup):** Dedicated push-button used to trigger the jump action in the Dino Game.
*   **Pin 12 (Digital Input):** Receives the verification signal from the secondary RFID Arduino.

---

## 📌 System 2: RFID Access Control (Secondary Arduino)

This board runs independently to continuously scan for passive RFID tags. It converts raw hardware data into readable formats and triggers precise output states depending on whether the tag is authorized.

### Key Features
*   **Contactless Scanning:** Constantly polls for 13.56 MHz High-Frequency transponders via SPI communication.
*   **HEX String Conversion:** Parses raw byte arrays from the card's UID chips, adding leading zeros where necessary.
*   **Identity Validation:** Performs a matching query against an hardcoded Master UID key (`730522a6`).
*   **Timed Pulse Outputs:** Flashes distinct digital output channels for 300ms depending on authentication results.

### Hardware Wiring Configuration
Connect the RC522 reader and status lines to your second Arduino board using this layout:

#### 1. MFRC522 RFID Reader
*   **VCC** -> 3.3V (*Warning: Do not connect to 5V; doing so will permanently damage the chip*)
*   **RST (Reset)** -> Digital Pin **9**
*   **GND** -> GND
*   **MISO** -> Digital Pin **12** (Hardware SPI Standard on Uno)
*   **MOSI** -> Digital Pin **11** (Hardware SPI Standard on Uno)
*   **SCK** -> Digital Pin **13** (Hardware SPI Standard on Uno)
*   **SDA / SS (Slave Select)** -> Digital Pin **10**

#### 2. Status Output Indicators
*   **Pin 8 (Digital Output):** Access Granted line ("yes"). Fires a 300ms high pulse when UID matches `730522a6`.
*   **Pin 7 (Digital Output):** Access Denied line ("no"). Fires a 300ms high pulse when an unknown UID is swiped.

---

## 🛠️ Required Libraries
Make sure to install the following official libraries via the **Arduino IDE Library Manager** before attempting to compile and compile the sketches:
*   `LiquidCrystal_I2C`
*   `Keypad`
*   `MFRC522` (by GitHub Community / Mifare)
