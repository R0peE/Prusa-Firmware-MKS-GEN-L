# Prusa-Firmware-MKS-GEN-L
Prusa Firmware for MKS GEN L v2.0 board with TCM2208 drivers in standalone mode

# Functions that work
- Setup wizard and XYZ calibration
- All movement
- Heating
- Fans
- Prusa Link and Pursa Connect with RPI on USB port
- Compatible with MK52 heatbed
- Printer is printing
- Multilang support

# Functions not working
- MMU connection (no port for it)
- Silent mode (no digipots)
- RPI port
- Fancheck disabled

# How to build and flash:
```
cd Prusa-Firmware-MKS-GEN-L
./utils/bootstrap.py
mkdir build
cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../cmake/AvrGcc.cmake -DFW_VARIANTS=MK25S-MKS-GEN-L
ninja MK25S-MKS-GEN-L_ENGLISH
avrdude -c stk500v2 -b 115200 -p atmega2560 -v -P /dev/ttyUSB0 -D -U ./MK25S-MKS-GEN-L_FW_3.14.0-BETA1_ENGLISH.hex
```
# Modified files
Every modification starts and ends with /*MKS*/
- adc.cpp
- boards.h
- config.h
- Marlin_main.cpp
- pins.h
- sm4.c
- swi2c.c
- temperature.cpp > OLD_BED_HEATER \
Added:
- pins_MKS_GEN_L.h
- variants/MK25S-MKS-GEN-L.h

# Thanks to
https://github.com/ghent360/Prusa-Firmware \
https://github.com/cimoalpacino/Prusa-Firmware \
https://github.com/prusa3d/Prusa-Firmware
