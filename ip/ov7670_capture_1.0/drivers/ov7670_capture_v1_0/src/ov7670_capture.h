
#ifndef OV7670_CAPTURE_H
#define OV7670_CAPTURE_H


/****************** Include Files ********************/
#include "xil_types.h"
#include "xstatus.h"
#include <stdio.h>

#define OV7670_CAPTURE_S00_AXI_SLV_REG0_OFFSET 0
#define OV7670_CAPTURE_S00_AXI_SLV_REG1_OFFSET 4
#define OV7670_CAPTURE_S00_AXI_SLV_REG2_OFFSET 8
#define OV7670_CAPTURE_S00_AXI_SLV_REG3_OFFSET 12


/**************************** Type Definitions *****************************/
typedef struct {
    UINTPTR BaseAddress;
    uint8_t enabled;
} ov7670_capture;

/**
 *
 * Write a value to a OV7670_CAPTURE register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the OV7670_CAPTUREdevice.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void OV7670_CAPTURE_mWriteReg(u32 BaseAddress, unsigned RegOffset, u32 Data)
 *
 */
#define OV7670_CAPTURE_mWriteReg(BaseAddress, RegOffset, Data) \
  	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))

/**
 *
 * Read a value from a OV7670_CAPTURE register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the OV7670_CAPTURE device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	u32 OV7670_CAPTURE_mReadReg(u32 BaseAddress, unsigned RegOffset)
 *
 */
#define OV7670_CAPTURE_mReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))

/************************** Function Prototypes ****************************/
/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the OV7670_CAPTURE instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus OV7670_CAPTURE_Reg_SelfTest(void * baseaddr_p);


/**
 * @brief Function to initialize a ov7670_capture instance to a given base address.
 * 
 * @param instance_ptr is a pointer to an ov7670_capture instance.
 * @param effective_addr is the base address of the peripheral.
 */
void ov7670_capture_initialize(ov7670_capture *instance_ptr, UINTPTR effective_addr);

/**
 * @brief Function to enable an ov7670_capture instance.
 * 
 * @param instance_ptr is a pointer to the ov7670 capture instance to enable.
 */
void ov7670_capture_enable(ov7670_capture *instance_ptr);


/**
 * @brief Function to disable an ov7670_capture instance.
 * 
 * @param instance_ptr is a pointer to the ov7670_capture instance to disable.
 */
void ov7670_capture_disable(ov7670_capture *instance_ptr);

#endif // OV7670_CAPTURE_H
