# Aftermarket-Speedometer-Pulse-Generator
Pure 5V Arduino Nano pulse generator for aftermarket motorcycle gauges (tach/speed). Bypass complex CAN/K-Line systems easily. Confirmed working on cheap gauges-no external 12V circuit required!
# MotoGaugePulse: Direct Pulse Generator for Aftermarket Motorcycle Gauges

## Project Introduction: Bridging the Gap Between CAN Bus and Analog Gauges

This repository documents a robust, low-cost engineering solution to a common problem in motorcycle customization: integrating inexpensive, pulse-driven aftermarket speedometers and tachometers onto modern bikes that utilize complex digital networks, specifically **Controller Area Network (CAN Bus)** or **K-Line** protocols.

While new motorcycles leverage the CAN Bus for efficient data transmission (sending digital packets for RPM, speed, temperature, etc.), generic aftermarket gauges require a traditional **frequency-based pulse signal** (a simple electrical ON/OFF square wave) to function. This project transforms an ordinary Arduino-compatible microcontroller into a dedicated, high-precision pulse generator, effectively simulating the sensor signals the gauge expects.

###  The Crucial Technical Discovery: 5V Direct-Drive Confirmed

The most significant finding of this project and a massive cost-saver - is the confirmation that the typical style of inexpensive aftermarket gauge will successfully read and operate accurately using the **pure 0V to 5V digital signal** output directly from an Arduino I/O pin.

This discovery entirely eliminates the need for any external components often required to interface 5V logic with 12V automotive systems (such as high-side switch transistors, optocouplers, or pull-up/pull-down resistor networks). The result is an exceptionally clean, stable, and minimalist solution for the custom community.

For a comprehensive breakdown of the project, including the theory behind the signal conversion and installation process, please refer to the detailed article:
**Full Blog Post:** [CAN Bus Bypass Using Only Arduino to Drive Aftermarket Speedometers](https://ocsaly.com/can-bus-bypass-using-only-arduino-to-drive-aftermarket-speedometers/)
https://ocsaly.com/can-bus-bypass-using-only-arduino-to-drive-aftermarket-speedometers/

---

##  How the Code Functions: Dual Independent Signal Generation

The core file, `arduino_pulse_generator.ino`, utilizes the microcontroller's high-resolution `micros()` function—essential for accurate timing at higher frequencies—to generate two completely separate square wave outputs. The code incorporates an automated sweep feature, allowing for continuous, hands-free testing across the full operational range of the gauges.

| Signal | Output Pin | Simulated Functionality | Calculation Basis |
| :--- | :--- | :--- | :--- |
| **Tachometer (RPM)** | Digital Pin **D8** | Sweeps smoothly between 1,000 RPM (Idle) and 7,500 RPM (Redline), testing low and high-end accuracy. | Calibrated for **1 Pulse Per Engine Revolution (1 PPR)**, typical for single-cylinder or wasted-spark ignition systems. |
| **Speedometer (Speed)** | Digital Pin **D9** | Sweeps road speed from a low-speed crawl (10 KPH) up to highway speed (160 KPH). | Precisely calibrated for a tire circumference of **1902 mm** to ensure accurate speed reporting. |

### Pulse Calculation Formulas

The code implements the following formulas internally to determine the precise timing intervals for the pulses:

**1. RPM Frequency Calculation:**
$$
\text{Frequency (Hz)} = \frac{\text{RPM}}{60 \text{ seconds}} \times \text{PPR} \quad \text{where PPR} = 1
$$

**2. Speedometer Frequency Calculation:**
First, the required Pulses Per Kilometer (PPK) is determined by the tire size:
$$
\text{PPK} = \frac{1,000,000 \text{ mm}}{\text{Circumference in mm}} \approx 526 \text{ pulses/km}
$$
Then, the operational frequency is calculated based on the target speed:
$$
\text{Frequency (Hz)} = \frac{\text{PPK} \times \text{Speed (km/h)}}{3600 \text{ seconds}}
$$

---

## Wiring and Implementation Guide (The Minimalist Approach)

This minimalist approach simplifies installation dramatically. You only need to identify three wires on the gauge harness and connect them to your Arduino board.

1.  **Power the Gauge:** The aftermarket gauge must be powered via its main **+12V Switched Power** and **Ground** leads connected to the motorcycle's battery or ignition circuit.
2.  **Establish Common Ground:** Connect an **Arduino GND** pin directly to the gauge's **Ground Wire**. This is non-negotiable for establishing the 0V reference point.
3.  **Connect RPM Signal:** Connect **Arduino Pin D8** directly to the tachometer's dedicated **RPM Signal Input Wire**.
4.  **Connect Speed Signal:** Connect **Arduino Pin D9** directly to the speedometer's **Speed Signal Input Wire**.

### 📸 Installation Visuals

The image below clearly illustrates the simplicity of the hardware setup, demonstrating the direct wiring of the three essential connections (GND, D8, D9) from the microcontroller to the gauge harness.
![alt text](https://github.com/typhoniks/Aftermarket-Speedometer-Pulse-Generator/blob/main/1.png?raw=true)
![alt text](https://github.com/typhoniks/Aftermarket-Speedometer-Pulse-Generator/blob/main/2.png?raw=true)
![alt text](https://github.com/typhoniks/Aftermarket-Speedometer-Pulse-Generator/blob/main/3.png?raw=true)


---

## 💻 Repository Contents

| File Name | Description |
| :--- | :--- |
| `arduino_pulse_generator.ino` | The final Arduino sketch containing the automated, calibrated, dual-signal sweep logic. |
| `README.md` | This extensive project documentation. |
| `image.png` | Supporting visual demonstrating the simple connection points. |

Full blog with explanations : https://ocsaly.com/can-bus-bypass-using-only-arduino-to-drive-aftermarket-speedometers/
