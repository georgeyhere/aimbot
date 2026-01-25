/**
 * @file tpg_driver.c
 * @brief Test Pattern Generator Hardware Abstraction Layer Implementation
 */

#include "tpg_driver.h"
#include "xil_printf.h"
#include "app_config.h"

int TpgDriver_Init(TpgDriver* driver, UINTPTR baseAddr)
{
    if (!driver) {
        return XST_FAILURE;
    }

    /* Lookup configuration */
    driver->config = XV_tpg_LookupConfig(baseAddr);
    if (!driver->config) {
        xil_printf("ERR: TPG configuration not found for base 0x%08X\r\n", (u32)baseAddr);
        return XST_DEVICE_NOT_FOUND;
    }

    /* Initialize the driver */
    int status = XV_tpg_CfgInitialize(&driver->instance, driver->config, driver->config->BaseAddress);
    if (status != XST_SUCCESS) {
        xil_printf("ERR: TPG initialization failed: %d\r\n", status);
        return XST_FAILURE;
    }

    driver->isInitialized = 1;
    driver->isEnabled = 0;
    driver->currentPattern = 0;
    driver->width = 0;
    driver->height = 0;

    return XST_SUCCESS;
}

int TpgDriver_Configure(TpgDriver* driver, u16 width, u16 height, u32 pattern)
{
    if (!driver || !driver->isInitialized) {
        return XST_FAILURE;
    }

    /* Configure dimensions */
    XV_tpg_Set_height(&driver->instance, height);
    XV_tpg_Set_width(&driver->instance, width);
    
    /* Configure pattern */
    XV_tpg_Set_colorFormat(&driver->instance, 0);
    XV_tpg_Set_bckgndId(&driver->instance, pattern);
    XV_tpg_Set_ovrlayId(&driver->instance, 0);
    
    /* Disable input (source mode) */
    XV_tpg_Set_enableInput(&driver->instance, 0);

    driver->width = width;
    driver->height = height;
    driver->currentPattern = pattern;

    return XST_SUCCESS;
}

int TpgDriver_ConfigurePassthrough(TpgDriver* driver, u16 width, u16 height, u32 pattern)
{
    if (!driver || !driver->isInitialized) {
        return XST_FAILURE;
    }

    /* Configure dimensions */
    XV_tpg_Set_height(&driver->instance, height);
    XV_tpg_Set_width(&driver->instance, width);
    
    /* Configure pattern and overlay */
    XV_tpg_Set_colorFormat(&driver->instance, 0);
    XV_tpg_Set_bckgndId(&driver->instance, pattern);
    XV_tpg_Set_ovrlayId(&driver->instance, 0);
    
    /* Enable passthrough */
    XV_tpg_Set_enableInput(&driver->instance, 1);
    XV_tpg_Set_passthruStartX(&driver->instance, 0);
    XV_tpg_Set_passthruStartY(&driver->instance, 0);
    XV_tpg_Set_passthruEndX(&driver->instance, width);
    XV_tpg_Set_passthruEndY(&driver->instance, height);

    driver->width = width;
    driver->height = height;
    driver->currentPattern = pattern;

    return XST_SUCCESS;
}

int TpgDriver_SetPattern(TpgDriver* driver, u32 pattern)
{
    if (!driver || !driver->isInitialized) {
        return XST_FAILURE;
    }

    if (pattern >= TPG_PATTERN_COUNT) {
        xil_printf("ERR: Invalid pattern ID %d (max %d)\r\n", pattern, TPG_PATTERN_COUNT - 1);
        return XST_FAILURE;
    }

    XV_tpg_Set_bckgndId(&driver->instance, pattern);
    driver->currentPattern = pattern;

    return XST_SUCCESS;
}

void TpgDriver_Start(TpgDriver* driver)
{
    if (driver && driver->isInitialized) {
        /* Write to AP_CTRL register to start TPG (auto-restart enabled) */
        XV_tpg_WriteReg(driver->config->BaseAddress, XV_TPG_CTRL_ADDR_AP_CTRL, 0x81);
        driver->isEnabled = 1;
    }
}

void TpgDriver_Stop(TpgDriver* driver)
{
    if (driver && driver->isInitialized) {
        /* Write to AP_CTRL register to stop TPG */
        XV_tpg_WriteReg(driver->config->BaseAddress, XV_TPG_CTRL_ADDR_AP_CTRL, 0x00);
        driver->isEnabled = 0;
    }
}

u16 TpgDriver_GetPixelsPerClock(TpgDriver* driver)
{
    if (driver && driver->isInitialized) {
        return driver->instance.Config.PixPerClk;
    }
    return 0;
}
