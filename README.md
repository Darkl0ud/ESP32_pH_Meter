# ESP32 pH Meter Platform
![Static Badge](https://img.shields.io/badge/status-broken/incomplete-red)

Uses PH-4502C pH controller board and SSD1306 screen to display pH refreshed on screen every second.

# How To Calibrate / How Calibration Works
Calibration is performed with a slope-intercept equation which varies between different pH ranges. Each pH range (0-4, 4-7, 7-10, 10-14) utilizes a different slope, 
which allows for multi-point calibration, rather than relying on one equation for calibration over the whole range.
