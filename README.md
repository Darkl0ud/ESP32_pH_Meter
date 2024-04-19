# ESP32 pH Meter
![Static Badge](https://img.shields.io/badge/Status-Development/Unreleased-yellow) <br>
Uses PH-4502C pH controller board and SSD1306 screen to display an average pH over one second, with the highest and lowest readings disacarded, refreshed on screen every second.

> [!WARNING]
> If you use this code, be sure to read this whole README before hand, as it contains important information for using the PH-4502C module.

## Libraries
This repo requires the **BusIO**, **SSD1306**, and **GFX** libraries provided by Adafruit.

## How Calibration Works
Calibration is performed with a slope-intercept equation which varies between different pH ranges. Each pH range (0-4, 4-7, 7-10, 10-14) utilizes a different slope, 
which allows for multi-point calibration, rather than relying on one equation for calibration over the whole range.

## About the 4502C
The PH-4502C is a pH probe amplifier that takes a BNC equipped pH probe and outputs 0-5v at the PO pin dependant upon pH of solution. <br>
### Pinout
TO – Temperature output<br>
DO – 3.3V pH limit trigger<br>
PO – PH analog output<br>
Gnd – Gnd for PH probe<br>
Gnd – Gnd for board<br>
VCC – 5V DC<br>
POT 1 – Analog reading offset (Nearest to BNC connector)<br>
POT 2 – PH limit setting<br>
<br>
![ph4502c](https://github.com/Darkl0ud/ESP32_pH_Meter/blob/main/README/ph4502c.jpg)

## TODO
- [ ] Reimplement SSD1306 LED Display
- [ ] pH changes with temperature, and currently there is no accounting or this within the code.
