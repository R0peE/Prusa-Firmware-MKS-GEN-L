/*****************************************************************
* MKS GEN L 2.0 Pin Assignments
******************************************************************/

#define ELECTRONICS "MKS_GEN_L"

#define KNOWN_BOARD
#ifndef __AVR_ATmega2560__
  #error Oops!  Make sure you have 'Arduino Mega 2560 or Rambo' selected from the 'Tools -> Boards' menu.
#endif

//#define UVLO_SUPPORT

//#define AMBIENT_THERMISTOR
//#define PINDA_THERMISTOR

//#define W25X20CL                 // external 256kB flash
//#define BOOTAPP                  // bootloader support


//#define SWI2C_SDA      20 //SDA on P3
//#define SWI2C_SCL      21 //SCL on P3

#define X_STEP_PIN          54 // A0
#define X_DIR_PIN           55 // A1
#define X_MIN_PIN           3
#define X_MAX_PIN           -1 //default 2
#define X_ENABLE_PIN        38
#define X_MS1_PIN           5
#define X_MS2_PIN           63

#define Y_STEP_PIN          60 // A6
#define Y_DIR_PIN           61 // A7
#define Y_MIN_PIN           14
#define Y_MAX_PIN           -1 //default 15
#define Y_ENABLE_PIN        56 // A2
#define Y_MS1_PIN           6
#define Y_MS2_PIN           64

#define Z_STEP_PIN          46
#define Z_DIR_PIN           48
#define Z_MIN_PIN           18
#define Z_MAX_PIN           -1 // default 19
#define Z_ENABLE_PIN        62 // A8
#define Z_MS1_PIN           19
#define Z_MS2_PIN           65

#define HEATER_BED_PIN       8
#define TEMP_BED_PIN         14 //A14

#define HEATER_0_PIN         10 //PE5
#define TEMP_0_PIN           13 //A13

#define HEATER_1_PIN        -1
#define TEMP_1_PIN          -1

#define HEATER_2_PIN        -1
#define TEMP_2_PIN          -1

#define TEMP_AMBIENT_PIN    -1 //A5

#define TEMP_PINDA_PIN      -1 //A15

//#define VOLT_PWR_PIN         4 //A4
//#define VOLT_BED_PIN         9 //A9

#define E0_STEP_PIN         26
#define E0_DIR_PIN          28
#define E0_ENABLE_PIN       24
#define E0_MS1_PIN          66
#define E0_MS2_PIN           2


#define SDPOWER             -1
#define SDSS                53
#define LED_PIN             13  // ???
#define FAN_PIN              9
#define FAN_1_PIN           -1
#define PS_ON_PIN           -1
#define SUICIDE_PIN         -1  // PIN that has to be turned on right after start, to keep power flowing.

#define BEEPER              37
#define LCD_PINS_RS         16
#define LCD_PINS_ENABLE     17
#define LCD_PINS_D4	        23
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
#define IR_SENSOR_PIN       19  // idler sensor @PK0 (digital pin 62/A8)

// This should be long enough to safely exit the bootloader when it uses the default timeout (~1-2s)
#define WATCHDOG_SOFT_RESET_VALUE WDTO_2S

#define TEMP_TIM 5
