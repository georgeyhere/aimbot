/**
 * @file vdma_driver.h
 * @brief VDMA Hardware Abstraction Layer
 */

#ifndef VDMA_DRIVER_H
#define VDMA_DRIVER_H

#include "xaxivdma.h"

/**
 * VDMA driver instance
 */
typedef struct {
    XAxiVdma instance;
    XAxiVdma_Config* config;
    XAxiVdma_DmaSetup readCfg;
    XAxiVdma_DmaSetup writeCfg;
    u32 frameBufferAddr;
    u16 width;
    u16 height;
    u16 stride;
    u8 frameCount;
    u8 isInitialized;
    u8 isRunning;
} VdmaDriver;

/**
 * Initialize VDMA driver
 * 
 * @param driver Pointer to VDMA driver instance
 * @param baseAddr Base address of VDMA in hardware
 * @return XST_SUCCESS or XST_FAILURE
 */
int VdmaDriver_Init(VdmaDriver* driver, UINTPTR baseAddr);

/**
 * Configure VDMA for video streaming
 * 
 * @param driver Pointer to VDMA driver instance
 * @param width Frame width in pixels
 * @param height Frame height in pixels
 * @param frameBufferAddr Base address of frame buffers in DDR
 * @param frameCount Number of frame buffers (typically 3)
 * @return XST_SUCCESS or XST_FAILURE
 */
int VdmaDriver_Configure(VdmaDriver* driver, u16 width, u16 height, 
                         u32 frameBufferAddr, u8 frameCount);

/**
 * Start VDMA transfers
 * 
 * @param driver Pointer to VDMA driver instance
 * @return XST_SUCCESS or XST_FAILURE
 */
int VdmaDriver_Start(VdmaDriver* driver);

/**
 * Stop VDMA transfers
 * 
 * @param driver Pointer to VDMA driver instance
 * @return XST_SUCCESS or XST_FAILURE
 */
int VdmaDriver_Stop(VdmaDriver* driver);

/**
 * Reset VDMA
 * 
 * @param driver Pointer to VDMA driver instance
 */
void VdmaDriver_Reset(VdmaDriver* driver);

#endif /* VDMA_DRIVER_H */
