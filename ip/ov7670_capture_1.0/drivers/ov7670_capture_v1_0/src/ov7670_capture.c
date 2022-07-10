

/***************************** Include Files *******************************/
#include "ov7670_capture.h"

/************************** Function Definitions ***************************/


/**
 * @brief Function to initialize a ov7670_capture instance to a given base address.
 * 
 * @param instance_ptr is a pointer to an ov7670_capture instance.
 * @param effective_addr is the base address of the peripheral.
 */
void ov7670_capture_initialize(ov7670_capture *instance_ptr, UINTPTR effective_addr)
{
    instance_ptr->BaseAddress = effective_addr;
}

/**
 * @brief Function to enable an ov7670_capture instance.
 * 
 * @param instance_ptr is a pointer to the ov7670 capture instance to enable.
 */
void ov7670_capture_enable(ov7670_capture *instance_ptr)
{
    OV7670_CAPTURE_mWriteReg(instance_ptr->BaseAddress, 0, 1);
    instance_ptr->enabled = 1;
}


/**
 * @brief Function to disable an ov7670_capture instance.
 * 
 * @param instance_ptr is a pointer to the ov7670_capture instance to disable.
 */
void ov7670_capture_disable(ov7670_capture *instance_ptr)
{
    OV7670_CAPTURE_mWriteReg(instance_ptr->BaseAddress, 0, 0);
    instance_ptr->enabled = 0;
}