#include "i2c_master.h"
#include "i2c_master_l.h"

/*
start write to address
	- cmd_address = slave_addr
	- cmd_start   = 1

write 8-bit data
	- cmd_write   = 1
	- cmd_stop    = *
	- cmd_valid   = 1
	- tdata       = 8-bit data

standalone stop
	- cmd_start   = 0
	- cmd_write   = 0
	- cmd_stop    = 1

read
	- cmd_start   = 1
	- cmd_write   = 0
	- cmd_read    = 1
	- cmd_stop    = *

write 0x11223344 to register 0x0004 on device at 0x50
01 1010000  start write to 0x50
1 00000000  write address 0x0004
1 00000100
1 00010001  write data 0x11223344
1 00100010
1 00110011
1 01000100
0 00000000  stop
*/

/**
 * Function to send a standalone i2c start condition consisting of the i2c slave
 * address and read/write bit.
 *
 * @param instance_ptr is a pointer to an i2c_device instance.
 */
void i2c_start_condition(i2c_device *instance_ptr)
{
	uint32_t reg_val;

	// get current control register
	reg_val = I2C_GET_CMD_REG(instance_ptr->reg_base_address);

	// clear cmd_stop, then set cmd_start and i2c slave address
	reg_val &= ~(I2C_CMD_REG_STOP);
	reg_val |= I2C_CMD_REG_START;
	reg_val |= (instance_ptr->config.slave_addr & I2C_CMD_REG_ADDR_MASK);

	// write to control register
	I2C_SET_CMD_REG(instance_ptr->reg_base_address, reg_val);
}


/**
 * Function to send a byte to an i2c slave.
 *
 * @param instance_ptr
 * @param stop
 */
int i2c_write_byte(i2c_device *instance_ptr, uint8_t data, _Bool start, _Bool stop)
{
	uint32_t reg_val;

	// get current control register
	reg_val = I2C_GET_CMD_REG(instance_ptr->reg_base_address);

	// set or clear cmd_start
	if(start) reg_val |= I2C_CMD_REG_START;
	else      reg_val &= ~(I2C_CMD_REG_START);

	// set or clear cmd_stop
	if(stop) reg_val |= I2C_CMD_REG_STOP;
	else     reg_val &= ~(I2C_CMD_REG_STOP);

	// push data fifo
	I2C_SET_DATA_REG(instance_ptr->reg_base_address, (I2C_DATA_REG_VALID | data));

	// push command
	I2C_SET_CMD_REG(instance_ptr->reg_base_address, reg_val);

	return XST_SUCCESS; //TODO
}

/**
 * Function to send a standalone i2c stop condition.
 *
 * @param instance_ptr is a pointer to an i2c_device instance.
 */
void i2c_stop_condition(i2c_device *instance_ptr)
{
	uint32_t reg_val;

	// get current control register
	reg_val = I2C_GET_CMD_REG(instance_ptr->reg_base_address);

	// clear cmd_start, cmd_wr, and cmd_rd, then set cmd_stop
	reg_val &= ~(I2C_CMD_REG_START | I2C_CMD_REG_WR_EN | I2C_CMD_REG_RD_EN);
	reg_val |= I2C_CMD_REG_STOP;

	// write to control register
	I2C_SET_CMD_REG(instance_ptr->reg_base_address, reg_val);
}










