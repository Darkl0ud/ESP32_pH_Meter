# ESP32 pH Meter
![Static Badge](https://img.shields.io/badge/Status-Deep_Development/Works-yellow)

Uses PH-4502C pH controller board and SSD1306 screen to display pH averaged over one second with the highest and lowest values disacarded, refreshed on screen every second.

## Libraries
This repo requires the **BusIO**, **SSD1306**, and **GFX** libraries provided by Adafruit.

## How To Calibrate / How Calibration Works
Calibration is performed with a slope-intercept equation which varies between different pH ranges. Each pH range (0-4, 4-7, 7-10, 10-14) utilizes a different slope, 
which allows for multi-point calibration, rather than relying on one equation for calibration over the whole range.

## How To Make It Better
pH changes with temperature, and currently there is no accounting or this within the code.
