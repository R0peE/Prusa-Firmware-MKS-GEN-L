//sm4.c - simple 4-axis stepper control

#include "sm4.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <math.h>
#include "Arduino.h"

#include "boards.h"
#define false 0
#define true 1
#include "Configuration_var.h"


#ifdef NEW_XYZCAL


// Signal pinouts

// direction signal - MiniRambo
//#define X_DIR_PIN    48 //PL1 (-)
//#define Y_DIR_PIN    49 //PL0 (-)
//#define Z_DIR_PIN    47 //PL2 (-)
//#define E0_DIR_PIN   43 //PL6 (+)

//direction signal - EinsyRambo
//#define X_DIR_PIN    49 //PL0 (+)
//#define Y_DIR_PIN    48 //PL1 (-)
//#define Z_DIR_PIN    47 //PL2 (+)
//#define E0_DIR_PIN   43 //PL6 (-)

//step signal pinout - common for all rambo boards
//#define X_STEP_PIN   37 //PC0 (+)
//#define Y_STEP_PIN   36 //PC1 (+)
//#define Z_STEP_PIN   35 //PC2 (+)
//#define E0_STEP_PIN  34 //PC3 (+)

// MKS GEN L 2.0
// X_DIR_PIN 		A1 - PF1
// Y_DIR_PIN 		A7 - PF7
// Z_DIR_PIN 		D48- PL1
// E0_DIR_PIN 		D28- PA6
// E1_DIR_PIN 		D34- PC3

// X_STEP_PIN 		A0 - PF0
// Y_STEP_PIN 		A6 - PF6
// Z_STEP_PIN 		D46- PL3
// E0_STEP_PIN 		D26- PA4
// E1_STEP_PIN 		D36- PC1


#define XDIR INVERT_X_DIR:!INVERT_X_DIR
#define YDIR INVERT_Y_DIR:!INVERT_Y_DIR
#define ZDIR INVERT_Z_DIR:!INVERT_Z_DIR
#define EDIR INVERT_E0_DIR:!INVERT_E0_DIR

uint8_t dir_mask = 0x0F^(INVERT_X_DIR | (INVERT_Y_DIR << 1) | (INVERT_Z_DIR << 2) | (INVERT_E0_DIR << 3));

sm4_stop_cb_t sm4_stop_cb = 0;

sm4_update_pos_cb_t sm4_update_pos_cb = 0;

sm4_calc_delay_cb_t sm4_calc_delay_cb = 0;

uint16_t sm4_cpu_time = 0;


uint8_t sm4_get_dir(uint8_t axis)
{
	switch (axis)
	{
#if ((MOTHERBOARD == BOARD_RAMBO_MINI_1_0) || (MOTHERBOARD == BOARD_RAMBO_MINI_1_3))
	case 0: return (PORTL & 2)?XDIR;
	case 1: return (PORTL & 1)?YDIR;
	case 2: return (PORTL & 4)?ZDIR;
	case 3: return (PORTL & 64)?EDIR;
#elif ((MOTHERBOARD == BOARD_EINSY_1_0a))
	case 0: return (PORTL & 1)?XDIR;
	case 1: return (PORTL & 2)?YDIR;
	case 2: return (PORTL & 4)?ZDIR;
	case 3: return (PORTL & 64)?EDIR;
/*MKS*/	
#elif (MOTHERBOARD == BOARD_MKS_GEN_L)
	case 0: return (PORTF & 2)?XDIR; // X
	case 1: return (PORTF & 0x80)?YDIR;  // Y
	case 2: return (PORTL & 2)?ZDIR;  // Z
	case 3: return (PORTA & 0x40)?EDIR; // E
#endif /*MKS*/
	}
	return 0;
}

void sm4_set_dir(uint8_t axis, uint8_t dir)
{
	switch (axis)
	{
#if ((MOTHERBOARD == BOARD_RAMBO_MINI_1_0) || (MOTHERBOARD == BOARD_RAMBO_MINI_1_3))
	case 0: if (dir == INVERT_X_DIR) PORTL |= 2; else PORTL &= ~2; break;
	case 1: if (dir == INVERT_Y_DIR) PORTL |= 1; else PORTL &= ~1; break;
	case 2: if (dir == INVERT_Z_DIR) PORTL |= 4; else PORTL &= ~4; break;
	case 3: if (dir == INVERT_E0_DIR) PORTL |= 64; else PORTL &= ~64; break;
#elif ((MOTHERBOARD == BOARD_EINSY_1_0a))
	case 0: if (dir == INVERT_X_DIR) PORTL |= 1; else PORTL &= ~1; break;
	case 1: if (dir == INVERT_Y_DIR) PORTL |= 2; else PORTL &= ~2; break;
	case 2: if (dir == INVERT_Z_DIR) PORTL |= 4; else PORTL &= ~4; break;
	case 3: if (dir == INVERT_E0_DIR) PORTL |= 64; else PORTL &= ~64; break;
/*MKS*/
#elif (MOTHERBOARD == BOARD_MKS_GEN_L)
	case 0: if (dir == INVERT_X_DIR) PORTF |= 2; else PORTF &= ~2; break;
	case 1: if (dir == INVERT_Y_DIR) PORTF |= 0x80; else PORTF &= ~0x80; break;
	case 2: if (dir == INVERT_Z_DIR) PORTL |= 2; else PORTL &= ~2; break;
	case 3: if (dir == INVERT_E0_DIR) PORTA |= 0x40; else PORTA &= ~0x40; break;
	//case 4: if (!dir) PORTC |= 8; else PORTC &= ~8; break;
#endif /*MKS*/
	}
	asm("nop");
}

uint8_t sm4_get_dir_bits(void)
{
    register uint8_t dir_bits = 0;
	//TODO -optimize in asm
#if ((MOTHERBOARD == BOARD_RAMBO_MINI_1_0) || (MOTHERBOARD == BOARD_RAMBO_MINI_1_3))
    register uint8_t portL = PORTL;
	if (portL & 2) dir_bits |= 1;
	if (portL & 1) dir_bits |= 2;
	if (portL & 4) dir_bits |= 4;
	if (portL & 64) dir_bits |= 8;
	dir_bits ^= dir_mask;
#elif ((MOTHERBOARD == BOARD_EINSY_1_0a))
    register uint8_t portL = PORTL;
	if (portL & 1) dir_bits |= 1;
	if (portL & 2) dir_bits |= 2;
	if (portL & 4) dir_bits |= 4;
	if (portL & 64) dir_bits |= 8;
	dir_bits ^= dir_mask;
/*MKS*/
#elif (MOTHERBOARD == BOARD_MKS_GEN_L)
    register uint8_t portF = PORTF;
	if (portF & 2) dir_bits |= 1;
	if (portF & 0x80) dir_bits |= 2;
	if (PORTL & 2) dir_bits |= 4;
	if (PORTA & 0x40) dir_bits |= 8;
	dir_bits ^= dir_mask; 
/*MKS*/
#endif
	return dir_bits;
}

void sm4_set_dir_bits(uint8_t dir_bits)
{
	//TODO -optimize in asm
#if ((MOTHERBOARD == BOARD_RAMBO_MINI_1_0) || (MOTHERBOARD == BOARD_RAMBO_MINI_1_3))
    register uint8_t portL = PORTL;
	portL &= 0xb8; //set direction bits to zero
	dir_bits ^= dir_mask;
	if (dir_bits & 1) portL |= 2;  //set X direction bit
	if (dir_bits & 2) portL |= 1;  //set Y direction bit
	if (dir_bits & 4) portL |= 4;  //set Z direction bit
	if (dir_bits & 8) portL |= 64; //set E direction bit
	PORTL = portL;
#elif (MOTHERBOARD == BOARD_EINSY_1_0a)
    register uint8_t portL = PORTL;
	portL &= 0xb8; //set direction bits to zero
	dir_bits ^= dir_mask;
	if (dir_bits & 1) portL |= 1;  //set X direction bit
	if (dir_bits & 2) portL |= 2;  //set Y direction bit
	if (dir_bits & 4) portL |= 4;  //set Z direction bit
	if (dir_bits & 8) portL |= 64; //set E direction bit
	PORTL = portL;
/*MKS*/
#elif (MOTHERBOARD == BOARD_MKS_GEN_L)
    register uint8_t portF = PORTF & (~(0x80 | 0x02));
	dir_bits ^= dir_mask;
	if (dir_bits & 1) portF |= 2;						//set X direction bit
	if (dir_bits & 2) portF |= 0x80;					//set Y direction bit
	if (dir_bits & 4) PORTL |= 2; else PORTL &= ~2;		//set Z direction bit
	if (dir_bits & 8) PORTA |= 0x40; else PORTA &= ~0x40; //set E direction bit
	PORTF = portF;
/*MKS*/
#endif
	asm("nop");
}

void sm4_do_step(uint8_t axes_mask)
{
#if ((MOTHERBOARD == BOARD_RAMBO_MINI_1_0) || (MOTHERBOARD == BOARD_RAMBO_MINI_1_3) || (MOTHERBOARD == BOARD_EINSY_1_0a))
#ifdef TMC2130_DEDGE_STEPPING
	PINC = (axes_mask & 0x0f); // toggle step signals by mask
#else
    register uint8_t portC = PORTC & 0xf0;
	PORTC = portC | (axes_mask & 0x0f); //set step signals by mask
	asm("nop");
	PORTC = portC; //set step signals to zero
	asm("nop");
#endif
/*MKS*/
#elif (MOTHERBOARD == BOARD_MKS_GEN_L)
#ifdef TMC2130_DEDGE_STEPPING
	register uint8_t maskF = 0;
	register uint8_t maskL = 0;
	register uint8_t maskA = 0;
	if (axes_mask & 1) maskF |= 1;
	if (axes_mask & 2) maskF |= 0x40;
	if (axes_mask & 4) maskL = 0x08;
	if (axes_mask & 8) maskA = 0x10;
	PINF = maskF;
	PINL = maskL;
	PINA = maskA;
#else
	register uint8_t portF = PORTF & (~(0x40 | 0x01));
	register uint8_t portL = PORTL & (~0x08);
	register uint8_t portA = PORTA & (~0x10);
	if (axes_mask & 1) portF |= 1;
	if (axes_mask & 2) portF |= 0x40;
	if (axes_mask & 4) portL |= 0x08;
	if (axes_mask & 8) portA |= 0x10;
	PORTF = portF;
	PORTL = portL;
	PORTA = portA;
	PORTF = portF & (~(0x40 | 0x01));
	PORTL = portL & (~0x08);
	PORTA = portA & (~0x10);
#endif // TMC2130_DEDGE_STEPPING
/*MKS*/
#endif //((MOTHERBOARD == BOARD_RAMBO_MINI_1_0) || (MOTHERBOARD == BOARD_RAMBO_MINI_1_3) || (MOTHERBOARD == BOARD_EINSY_1_0a))
}

uint16_t sm4_line_xyze_ui(uint16_t dx, uint16_t dy, uint16_t dz, uint16_t de)
{
	uint16_t dd = (uint16_t)(sqrt((float)(((uint32_t)dx)*dx + ((uint32_t)dy*dy) + ((uint32_t)dz*dz) + ((uint32_t)de*de))) + 0.5);
	uint16_t nd = dd;
	uint16_t cx = dd;
	uint16_t cy = dd;
	uint16_t cz = dd;
	uint16_t ce = dd;
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t z = 0;
	uint16_t e = 0;
	while (nd)
	{
		if (sm4_stop_cb && (*sm4_stop_cb)()) break;
		uint8_t sm = 0; //step mask
		if (cx <= dx)
		{
			sm |= 1;
			cx += dd;
			x++;
		}
		if (cy <= dy)
		{
			sm |= 2;
			cy += dd;
			y++;
		}
		if (cz <= dz)
		{
			sm |= 4;
			cz += dd;
			z++;
		}
		if (ce <= de)
		{
			sm |= 8;
			ce += dd;
			e++;
		}
		cx -= dx;
		cy -= dy;
		cz -= dz;
		ce -= de;
		sm4_do_step(sm);
		uint16_t delay = SM4_DEFDELAY;
		if (sm4_calc_delay_cb) delay = (*sm4_calc_delay_cb)(nd, dd);
		if (delay) delayMicroseconds(delay);
		nd--;
	}
	if (sm4_update_pos_cb) (*sm4_update_pos_cb)(x, y, z, e);
	return nd;
}

uint16_t sm4_line_xyz_ui(uint16_t dx, uint16_t dy, uint16_t dz){
	uint16_t dd = (uint16_t)(sqrt((float)(((uint32_t)dx)*dx + ((uint32_t)dy*dy) + ((uint32_t)dz*dz))) + 0.5);
	uint16_t nd = dd;
	uint16_t cx = dd;
	uint16_t cy = dd;
	uint16_t cz = dd;
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t z = 0;
	while (nd){
		if (sm4_stop_cb && (*sm4_stop_cb)()) break;
		uint8_t sm = 0; //step mask
		if (cx <= dx){
			sm |= 1;
			cx += dd;
			x++;
		}
		if (cy <= dy){
			sm |= 2;
			cy += dd;
			y++;
		}
		if (cz <= dz){
			sm |= 4;
			cz += dd;
			z++;
		}
		cx -= dx;
		cy -= dy;
		cz -= dz;
		sm4_do_step(sm);
		uint16_t delay = SM4_DEFDELAY;
		if (sm4_calc_delay_cb) delay = (*sm4_calc_delay_cb)(nd, dd);
		if (delay) delayMicroseconds(delay);
		nd--;
	}
	if (sm4_update_pos_cb)
		(*sm4_update_pos_cb)(x, y, z, 0);
	return nd;
}

#endif //NEW_XYZCAL
