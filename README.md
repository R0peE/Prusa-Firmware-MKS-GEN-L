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
- MMU connection as (no port for it)
- Silent mode (no digipots)
- RPI port

# How to build and flash:
```
mkdir
cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../cmake/AvrGcc.cmake -DFW_VARIANTS=MK25S-MKS-GEN-L
ninja MK25S-MKS-GEN-L_ENGLISH
avrdude -c stk500v2 -b 115200 -p atmega2560 -v -P /dev/ttyUSB0 -D -U ./MK25S-MKS-GEN-L_FW_3.14.0-BETA1_ENGLISH.hex
```
