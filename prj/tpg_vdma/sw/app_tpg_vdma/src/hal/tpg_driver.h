/**
 * @file tpg_driver.h
 * @brief Test Pattern Generator Hardware Abstraction Layer
 */

#ifndef TPG_DRIVER_H
#define TPG_DRIVER_H

#include "xv_tpg.h"

/**
 * TPG driver instance
 */
typedef struct {
    XV_tpg instance;
    XV_tpg_Config* config;
    u32 currentPattern;
    u16 width;
    u16 height;
    u8 isInitialized;
    u8 isEnabled;
} TpgDriver;

/**
 * Initialize TPG driver
 * 
 * @param driver Pointer to TPG driver instance
 * @param baseAddr Base address of TPG in hardware
 * @return XST_SUCCESS or XST_FAILURE
 */
int TpgDriver_Init(TpgDriver* driver, UINTPTR baseAddr);

/**
 * Configure TPG for a specific video mode
 * 
 * @param driver Pointer to TPG driver instance
 * @param width Frame width in pixels
 * @param height Frame height in pixels
 * @param pattern Pattern ID (XTPG_BKGND_*)
 * @return XST_SUCCESS or XST_FAILURE
 */
int TpgDriver_Configure(TpgDriver* driver, u16 width, u16 height, u32 pattern);

/**
 * Configure TPG for passthrough mode
 * 
 * @param driver Pointer to TPG driver instance
 * @param width Frame width in pixels
 * @param height Frame height in pixels
 * @param pattern Overlay pattern ID
 * @return XST_SUCCESS or XST_FAILURE
 */
int TpgDriver_ConfigurePassthrough(TpgDriver* driver, u16 width, u16 height, u32 pattern);

/**
 * Set TPG pattern
 * 
 * @param driver Pointer to TPG driver instance
 * @param pattern Pattern ID (0-20)
 * @return XST_SUCCESS or XST_FAILURE
 */
int TpgDriver_SetPattern(TpgDriver* driver, u32 pattern);

/**
 * Start/Enable TPG
 * 
 * @param driver Pointer to TPG driver instance
 */
void TpgDriver_Start(TpgDriver* driver);

/**
 * Stop/Disable TPG
 * 
 * @param driver Pointer to TPG driver instance
 */
void TpgDriver_Stop(TpgDriver* driver);

/**
 * Get pixels per clock configured for this TPG
 * 
 * @param driver Pointer to TPG driver instance
 * @return Pixels per clock value
 */
u16 TpgDriver_GetPixelsPerClock(TpgDriver* driver);

#endif /* TPG_DRIVER_H */
