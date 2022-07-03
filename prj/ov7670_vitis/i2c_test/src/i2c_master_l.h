#ifndef IIC_MASTER_L_H
#define IIC_MASTER_L_H

/***************************** Include Files ********************************/
#include <stdio.h>
#include "xil_types.h"
#include "xil_assert.h"
#include "xil_io.h"

/************************** Constant Definitions ****************************/

/* i2c master register offsets */

#define I2C_STATUS_REG_OFFSET     0  /* Status Register, R/W */
#define I2C_CMD_REG_OFFSET        1
#define I2C_DATA_REG_OFFSET       2
#define I2C_PRESCALE_REG_OFFSET   3

/* Status Register bit positions */

#define I2C_STATUS_REG_RD_FULL       0x8000 /* read data FIFO full */
#define I2C_STATUS_REG_RD_EMPTY      0x4000 /* read data FIFO empty */
#define I2C_STATUS_REG_WR_OVF        0x2000 /* write data FIFO overflow; write 1 to clear */
#define I2C_STATUS_REG_WR_FULL       0x1000 /* write data FIFO full */
#define I2C_STATUS_REG_WR_EMPTY      0x800  /* write data FIFO empty */
#define I2C_STATUS_REG_CMD_OVF       0x400  /* command FIFO overflow; write 1 to clear */
#define I2C_STATUS_REG_CMD_FULL      0x200  /* command FIFO full */
#define I2C_STATUS_REG_CMD_EMPTY     0x100  /* command FIFO empty */
#define I2C_STATUS_REG_MISS_ACK      0x4    /* missed ACK from slave; write 1 to clear */
#define I2C_STATUS_REG_BUS_ACT       0x2    /* bus is active */
#define I2C_STATUS_REG_BUS_CONT      0x1    /* module has control of active bus */
#define I2C_STATUS_REG_BUSY          0x0    /* i2c operation ongoing */

/* Command Register bit positions */
/*** all non-data bits push on command FIFO when written to! ***/

#define I2C_CMD_REG_STOP             0x1000  /* set high to issue i2c stop condition */
#define I2C_CMD_REG_WR_MULTIPLE      0x800   /* set high to start block write */
#define I2C_CMD_REG_WR_EN            0x400   /* set high to start write */
#define I2C_CMD_REG_RD_EN            0x200   /* set high to start read */
#define I2C_CMD_REG_START            0x100   /* set high to issue i2c start condition */
#define I2C_CMD_REG_ADDR_MASK        0x7F    /* mask for 7-bit i2c address */


/* Data Register bit positions */
/*** XIIC_DATA_LAST and XIIC_DATA_VALID must be accessed w/ atomic 16-bit reads/writes! ***/

#define I2C_DATA_REG_LAST            0x200   /* indicates last byte of block write (write multiple) */
#define I2C_DATA_REG_VALID           0x100   /* indicates valid read data */
#define I2C_DATA_REG_MASK            0xFF    /* mask for 8-bit i2c data */



/* Prescale Register bit positions */

#define I2C_PRESCALE                 0xFFFF  /* mask for 16-bit prescale value (input clk cycles) */



/***************** Macros (Base Operations) Definitions ********************/
#define I2C_OUT32 Xil_Out32
#define I2C_IN32  Xil_In32

#define I2C_WRITE_REG(base_address, reg_offset, data) \
        I2C_OUT32((base_address) + (reg_offset), (u32)(data))

#define I2C_READ_REG(base_address, reg_offset) \
	    I2C_IN32((base_address) + (reg_offset))

#define I2C_SET_CMD_REG(base_address, mask) \
	    I2C_WRITE_REG((base_address), I2C_CMD_REG_OFFSET, (mask))

#define I2C_SET_DATA_REG(base_address, mask) \
	    I2C_WRITE_REG((base_address), I2C_DATA_REG_OFFSET, (mask))

#define I2C_SET_PRESCALE_REG(base_address, mask) \
	    I2C_WRITE_REG((base_address), I2C_PRESCALE_REG_OFFSET, (mask))

#define I2C_GET_STATUS_REG(base_address) \
	    I2C_READ_REG((base_address), I2C_STATUS_REG_OFFSET)

#define I2C_GET_CMD_REG(base_address) \
	    I2C_READ_REG((base_address), I2C_CMD_REG_OFFSET)

/***************** Macros (Register Checks) Definitions ********************/

/**
 * Check to see if there is data in the i2c receive buffer.
 * @param  BaseAddress is the base address of the device.
 * @return TRUE if the receiver is empty, FALSE if there is data present.
 */
#define I2C_IS_RECV_EMPTY(base_address) \
	    ((I2C_GET_STATUS_REG((base_address)) & I2C_DATA_VALID) != 1)

/**
 * Check to see if the i2c write data buffer is full.
 * @param  BaseAddress is the base address of the device.
 * @return TRUE if the buffer is full, FALSE otherwise.
 */
#define I2C_IS_WR_FULL(base_address) \
	    ((I2C_GET_STATUS_REG((base_address)) & I2C_WR_FULL) != 1)

/**
 * Check to see if the i2c command buffer is full.
 * @param  BaseAddress is the base address of the device.
 * @return TRUE if the buffer is full, FALSE otherwise.
 */
#define I2C_IS_CMD_FULL(base_address) \
	    ((I2C_GET_STATUS_REG((base_address)) & I2C_CMD_FULL) != 1)


#endif