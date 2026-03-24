# mmWave playKit
mmwave sensing project using seeedstudio 24GHz mmwave sensor and xiao esp32-s3

### Firmware notice
The included firmware is not fine tuned as of now because this project isn't built in real life as of yet. Also the included firmware is for controlling a different project of mine [Remote Occupancy Indicator](https://github.com/m1cha3l-s/remote-occupancy-indicator). The idea behind this project is to explore what's possible with this and try it in real world conditions so that you can have a better idea in which scenarios this sensor is usefull and in which it would lack functionality, with this idea comes the need of your own firmware to use it in your situation!

## Building
1. Solder the headers included with the module together with the PCB and ESP32. (Make sure the headers are on the sides with the silkscreen text)
2. Solder the headers from the ESP32 onto the module.
3. Solder the LEDs onto the PCB.
4. Connect the module to the headers on the PCB.
5. Place it inside the optional 3D Printed case.
6. Now you are finished!!!

## Flashing (Sensor for [Remote Occupancy Indicator](https://github.com/m1cha3l-s/remote-occupancy-indicator))
1. Flash the MAC-Finder in Arduino IDE (It can be found in the folder with the firmware).
2. Look into the serial monitor and write down the MAC address. (We will need this to be able to communicate with the Indicator.
3. For this demo to work we will need to reflash the Indicator with the MAC of the sensor.
4. After doing that flash the sensor with the MAC of the indicator.
5. Now you are finished!!! The sensor and indicator will connect automatically.

## PCB/Schematic
![PCB](https://github.com/m1cha3l-s/mmWave/blob/main/Pictures/pcb.png)
![Schematic 1](https://github.com/m1cha3l-s/mmWave/blob/main/Pictures/schematic1.png)

### BOM
| Part Name | Quantity | Price (USD) | Total (USD) | Link |
|---|---|---|---|---|
| XIAO ESP32-S3 | 1 | 10 | 10 | https://botland.cz/moduly-wifi-a-bt-esp32/22878-seeed-xiao-esp32-s3-wifi-bluetooth-seeedstudio-113991114.html |
| 24Ghz mmwave seeedstudio sensor | 1 | 6.70 | 6.70 | https://botland.cz/pohybove-senzory/24129-human-static-presence-mmwave-24ghz-senzor-pro-seeed-xiao-seeedstudio-101010001.html |
| botland shipping | 1 | 3.60 | 3.60 | |
| Custom PCB (with shipping) | 1 | 4.36 | 4.36 | jlcpcb |
| 3D Printed case | 1 | 0 | 0 | none - I have my own printer |
| Total ||| 24.66 |
