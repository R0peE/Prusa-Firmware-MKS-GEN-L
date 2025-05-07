/*****************************************************************
* FYSETC F6 1.3/1.4 Pin Assignments
******************************************************************/

#define ELECTRONICS "FYSETC_F6"

#define KNOWN_BOARD
#ifndef __AVR_ATmega2560__
  #error Oops!  Make sure you have 'Arduino Mega 2560 or Rambo' selected from the 'Tools -> Boards' menu.
#endif

//#define UVLO_SUPPORT

//#define AMBIENT_THERMISTOR
//#define PINDA_THERMISTOR

//#define W25X20CL                 // external 256kB flash
//#define BOOTAPP                  // bootloader support


#define SWI2C_SDA      20 //SDA on P3
#define SWI2C_SCL      21 //SCL on P3

#define X_STEP_PIN          54
#define X_DIR_PIN           55
#define X_MIN_PIN           63
#define X_MAX_PIN           -1 //default 64
#define X_ENABLE_PIN        38
#define X_MS1_PIN           71
#define X_MS2_PIN           72

#define Y_STEP_PIN          60
#define Y_DIR_PIN           61
#define Y_MIN_PIN           14
#define Y_MAX_PIN           -1 //default 15
#define Y_ENABLE_PIN        56
#define Y_MS1_PIN           73
#define Y_MS2_PIN           75

#define Z_STEP_PIN          43
#define Z_DIR_PIN           48
#define Z_MIN_PIN           12
#define Z_MAX_PIN           -1 // default 9
#define Z_ENABLE_PIN        58
#define Z_MS1_PIN           78
#define Z_MS2_PIN           79

#define HEATER_BED_PIN       8
#define TEMP_BED_PIN         15 //A15

#define HEATER_0_PIN         5 //PE3
#define TEMP_0_PIN           12 //A12

#define HEATER_1_PIN        -1
#define TEMP_1_PIN          -1

#define HEATER_2_PIN        -1
#define TEMP_2_PIN          -1

#define TEMP_AMBIENT_PIN    -1 //A5

#define TEMP_PINDA_PIN      7 //A7

// #define VOLT_PWR_PIN         4 //A4
// #define VOLT_BED_PIN         9 //A9

#define E0_STEP_PIN         26
#define E0_DIR_PIN          28
#define E0_ENABLE_PIN       24
#define E0_MS1_PIN          76
#define E0_MS2_PIN          77


#define SDPOWER             -1
#define SDSS                53
#define LED_PIN             13  // ???
#define FAN_PIN             45
#define FAN_1_PIN           -1
#define PS_ON_PIN           -1
#define SUICIDE_PIN         -1  // PIN that has to be turned on right after start, to keep power flowing.

#define BEEPER              37
#define LCD_PINS_RS         16
#define LCD_PINS_ENABLE     17
#define LCD_PINS_D4         23
#define LCD_PINS_D5         25
#define LCD_PINS_D6         27
#define LCD_PINS_D7         29

//buttons are directly attached using AUX-2
#define BTN_EN1             33
#define BTN_EN2             31
#define BTN_ENC             35
#define KILL_PIN            41
#define SDCARDDETECT        49

//#define TACH_0              -1 // !!! changed from 81 (EINY03)
//#define TACH_1                 80 
#define IR_SENSOR_PIN       9  // idler sensor @PK0 (digital pin 62/A8)

// This should be long enough to safely exit the bootloader when it uses the default timeout (~1-2s)
#define WATCHDOG_SOFT_RESET_VALUE WDTO_2S

#define TEMP_TIM 5
