/**
 * @file vtc_driver.h
 * @brief Video Timing Controller Hardware Abstraction Layer
 */

#ifndef VTC_DRIVER_H
#define VTC_DRIVER_H

#include "xvtc.h"
#include "xvidc.h"

/**
 * VTC driver instance
 */
typedef struct {
    XVtc instance;
    XVtc_Config* config;
    XVtc_Timing timing;
    u8 isInitialized;
    u8 isEnabled;
} VtcDriver;

/**
 * Initialize VTC driver
 * 
 * @param driver Pointer to VTC driver instance
 * @param baseAddr Base address of VTC in hardware
 * @return XST_SUCCESS or XST_FAILURE
 */
int VtcDriver_Init(VtcDriver* driver, UINTPTR baseAddr);

/**
 * Configure VTC for a video mode
 * 
 * @param driver Pointer to VTC driver instance
 * @param videoMode Xvidc video mode structure
 * @param pixelsPerClk Pixels per clock (from TPG config)
 * @return XST_SUCCESS or XST_FAILURE
 */
int VtcDriver_Configure(VtcDriver* driver, XVidC_VideoMode videoMode, u16 pixelsPerClk);

/**
 * Enable VTC and start timing generation
 * 
 * @param driver Pointer to VTC driver instance
 */
void VtcDriver_Enable(VtcDriver* driver);

/**
 * Disable VTC
 * 
 * @param driver Pointer to VTC driver instance
 */
void VtcDriver_Disable(VtcDriver* driver);

/**
 * Enable register updates
 * 
 * @param driver Pointer to VTC driver instance
 */
void VtcDriver_RegUpdateEnable(VtcDriver* driver);

#endif /* VTC_DRIVER_H */
