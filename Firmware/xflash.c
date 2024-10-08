//xflash.c

#include "xflash.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "spi.h"
#include "fastio.h"

#ifdef XFLASH

#define _MFRID_W25X20CL    0xEF
#define _DEVID_W25X20CL    0x11

#define _MFRID_GD25Q20C    0xC8
#define _DEVID_GD25Q20C    0x11

#define _CMD_ENABLE_WR     0x06
#define _CMD_ENABLE_WR_VSR 0x50
#define _CMD_DISABLE_WR    0x04
#define _CMD_RD_STATUS_REG 0x05
#define _CMD_WR_STATUS_REG 0x01
#define _CMD_RD_DATA       0x03
#define _CMD_RD_FAST       0x0b
#define _CMD_RD_FAST_D_O   0x3b
#define _CMD_RD_FAST_D_IO  0xbb
#define _CMD_PAGE_PROGRAM  0x02
#define _CMD_SECTOR_ERASE  0x20
#define _CMD_BLOCK32_ERASE 0x52
#define _CMD_BLOCK64_ERASE 0xd8
#define _CMD_CHIP_ERASE    0xc7
#define _CMD_CHIP_ERASE2   0x60
#define _CMD_PWR_DOWN      0xb9
#define _CMD_PWR_DOWN_REL  0xab
#define _CMD_MFRID_DEVID   0x90
#define _CMD_MFRID_DEVID_D 0x92
#define _CMD_JEDEC_ID      0x9f
#define _CMD_RD_UID        0x4b

#define _CS_LOW() WRITE(XFLASH_PIN_CS, 0)
#define _CS_HIGH() WRITE(XFLASH_PIN_CS, 1)

//#define _SPI_TX swspi_tx
//#define _SPI_RX swspi_rx
#define _SPI_TX(b)   spi_txrx(b)
#define _SPI_RX()    spi_txrx(0xff)


int xflash_mfrid_devid(void);


int8_t xflash_init(void)
{
	_CS_HIGH();
	SET_OUTPUT(XFLASH_PIN_CS);
	XFLASH_SPI_ENTER();
	if (!xflash_mfrid_devid()) return 0;
	return 1;
}

void xflash_enable_wr(void)
{
	_CS_LOW();
	_SPI_TX(_CMD_ENABLE_WR);             // send command 0x06
	_CS_HIGH();
}

void xflash_disable_wr(void)
{
	_CS_LOW();
	_SPI_TX(_CMD_DISABLE_WR);            // send command 0x04
	_CS_HIGH();
}

uint8_t xflash_rd_status_reg(void)
{
	_CS_LOW();
	_SPI_TX(_CMD_RD_STATUS_REG);         // send command 0x90
	uint8_t val = _SPI_RX();             // receive value
	_CS_HIGH();
	return val;
}

static void xflash_send_cmdaddr(uint8_t cmd, uint32_t addr)
{
	_SPI_TX(cmd);                        // send command 0x03
	_SPI_TX(((uint8_t*)&addr)[2]);       // send addr bits 16..23
	_SPI_TX(((uint8_t*)&addr)[1]);       // send addr bits 8..15
	_SPI_TX(((uint8_t*)&addr)[0]);       // send addr bits 0..7
}

void xflash_rd_data(uint32_t addr, uint8_t* data, uint16_t cnt)
{
	_CS_LOW();
	xflash_send_cmdaddr(_CMD_RD_DATA, addr);
	while (cnt--)                        // receive data
		*(data++) = _SPI_RX();
	_CS_HIGH();
}

void xflash_page_program(uint32_t addr, uint8_t* data, uint16_t cnt)
{
	_CS_LOW();
	xflash_send_cmdaddr(_CMD_PAGE_PROGRAM, addr);
	while (cnt--)                        // send data
		_SPI_TX(*(data++));
	_CS_HIGH();
}

void xflash_multipage_program(uint32_t addr, uint8_t* data, uint16_t cnt)
{
    while(cnt)
    {
        xflash_enable_wr();
        _CS_LOW();
        xflash_send_cmdaddr(_CMD_PAGE_PROGRAM, addr);
        while(1)
        {
            // send data
            _SPI_TX(*(data++));
            if(!--cnt || !(++addr & 0xFF))
            {
                // on a page boundary or end of write
                _CS_HIGH();
                xflash_wait_busy();
                break;
            }
        }
    }
}

void xflash_page_program_P(uint32_t addr, uint8_t* data, uint16_t cnt)
{
	_CS_LOW();
    xflash_send_cmdaddr(_CMD_PAGE_PROGRAM, addr);
	while (cnt--)                        // send data
		_SPI_TX(pgm_read_byte(data++));
	_CS_HIGH();
}

void xflash_erase(uint8_t cmd, uint32_t addr)
{
	_CS_LOW();
    xflash_send_cmdaddr(cmd, addr);
	_CS_HIGH();
}

void xflash_sector_erase(uint32_t addr)
{
	return xflash_erase(_CMD_SECTOR_ERASE, addr);
}

void xflash_block32_erase(uint32_t addr)
{
	return xflash_erase(_CMD_BLOCK32_ERASE, addr);
}

void xflash_block64_erase(uint32_t addr)
{
	return xflash_erase(_CMD_BLOCK64_ERASE, addr);
}

void xflash_chip_erase(void)
{
	_CS_LOW();
	_SPI_TX(_CMD_CHIP_ERASE);            // send command 0xc7
	_CS_HIGH();
}


void xflash_rd_uid(uint8_t* uid)
{
	_CS_LOW();
	_SPI_TX(_CMD_RD_UID);                // send command 0x4b
	uint8_t cnt = 4;                     // 4 dummy bytes
	while (cnt--)                        // transmit dummy bytes
		_SPI_TX(0x00);
	cnt = 8;                             // 8 bytes UID
	while (cnt--)                        // receive UID
		uid[7 - cnt] = _SPI_RX();
	_CS_HIGH();
}

int xflash_mfrid_devid(void)
{
	_CS_LOW();
	_SPI_TX(_CMD_MFRID_DEVID);           // send command 0x90
	uint8_t cnt = 3;                     // 3 address bytes
	while (cnt--)                        // send address bytes
		_SPI_TX(0x00);
	uint8_t xflash_mfrid = _SPI_RX();  // receive mfrid
	uint8_t xflash_devid = _SPI_RX();  // receive devid
	_CS_HIGH();
	return
		((xflash_mfrid == _MFRID_W25X20CL) && (xflash_devid == _DEVID_W25X20CL)) ||
		((xflash_mfrid == _MFRID_GD25Q20C) && (xflash_devid == _DEVID_GD25Q20C));
}

void xflash_wait_busy(void)
{
	while (xflash_rd_status_reg() & XFLASH_STATUS_BUSY) ;
}

#endif //XFLASH
