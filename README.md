# ESP32 pH Meter
![Static Badge](https://img.shields.io/badge/Status-Development/Unreleased-yellow) <br>
Uses PH-4502C pH controller board and SSD1306 screen to display an average pH over one second, with the highest and lowest readings disacarded, refreshed on screen every second.

> [!WARNING]
> Be sure to read this whole README before hand, as it contains important information for using the PH-4502C module, including setting the on-board offset.

## Libraries
This repo requires the **BusIO**, **SSD1306**, and **GFX** libraries provided by Adafruit.

## Calibration
Calibration results are computed with a slope-intercept equation which varies between different pH ranges. Each pH range (0-4, 4-7, 7-10, 10-14) utilizes a different slope. There are default values in the code that will need to be changed to obtain accurate results.

## About the 4502C
The PH-4502C is a pH probe amplifier that takes a BNC equipped pH probe and outputs 0-5v at the PO pin dependant upon the pH of tested solution. It is equipped with an LED that will illuminate when the output voltage is over the threshold set by POT2, which also sets D0 LOW. It also requires offset calibration performed before use with POT1 as outlined below. <br>

### Pinout
TO – TEMPERATURE_OUTPUT<br>
DO – 3.3V_LIMIT_OUTPUT<br>
PO – PH_ANALOG_OUTPUT<br>
GND – GND_PH_PROBE<br>
GND – GND_BOARD<br>
VCC – 5V DC<br>
POT1 – ANALOG READING OFSET (Nearest to BNC connector)<br>
POT2 – PH LIMIT SETTING<br>
<br>
![ph4502c](https://github.com/Darkl0ud/ESP32_pH_Meter/blob/main/README/ph4502c.jpg)

## TODO
- [X] Reimplement SSD1306 LED Display
- [ ] Button Debounce
- [ ] pH changes with temperature, and currently there is no accounting for this within the code.
- [ ] Increase rate at which samples are taken... Multisampling may help obtain a more accurate reading.
