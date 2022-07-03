#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include <stdio.h>
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/**
 * The following data type keeps track of the statistics for the i2c Driver.
 */
typedef struct {
	uint32_t  bytes_transmitted;
	uint32_t  bytes_received;
	uint32_t  receive_overrun_errors;
} i2c_stats;



/**
 * The following data type contains configuration information for the device.
 */
typedef struct {
	UINTPTR   reg_base_addr;
	uint32_t  slave_addr;
	uint32_t  prescale_val;
} i2c_config;


/**
 * The following data type contains all i2c instance data. The user is required
 * to allocate a variable of this type for every i2c device in the system. A
 * pointer to a variable of this type is then passed to the driver API functions.
 */
typedef struct {
	i2c_stats  stats;
	i2c_config config;
	UINTPTR    reg_base_address;
	uint32_t   is_ready;
} i2c_device;


/************************** Function Prototypes *****************************/

//**** Initialization Functions in file i2c_master_init.c ****//

/**
 * Function to initialize an i2c_device instance.
 *
 * @param  instance_ptr is a pointer to the i2c_device instance to initialize.
 * @param  base_address is the base address of the i2c device.
 * @return XST_SUCCESS if successful, else XST_FAILURE.
 */
int i2c_initialize(i2c_device *instance_ptr, uint32_t base_address);


//**** Required Functions in file i2c_master.c ****//




#endif



















