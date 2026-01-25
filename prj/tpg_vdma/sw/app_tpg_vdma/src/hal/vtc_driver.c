/**
 * @file vtc_driver.c
 * @brief Video Timing Controller Hardware Abstraction Layer Implementation
 */

#include "vtc_driver.h"
#include "xil_printf.h"

int VtcDriver_Init(VtcDriver* driver, UINTPTR baseAddr)
{
    if (!driver) {
        return XST_FAILURE;
    }

    /* Lookup configuration */
    driver->config = XVtc_LookupConfig(baseAddr);
    if (!driver->config) {
        xil_printf("ERR: VTC configuration not found for base 0x%08X\r\n", (u32)baseAddr);
        return XST_DEVICE_NOT_FOUND;
    }

    /* Initialize the driver */
    int status = XVtc_CfgInitialize(&driver->instance, driver->config, driver->config->BaseAddress);
    if (status != XST_SUCCESS) {
        xil_printf("ERR: VTC initialization failed: %d\r\n", status);
        return XST_FAILURE;
    }

    driver->isInitialized = 1;
    driver->isEnabled = 0;

    return XST_SUCCESS;
}

int VtcDriver_Configure(VtcDriver* driver, XVidC_VideoMode videoMode, u16 pixelsPerClk)
{
    if (!driver || !driver->isInitialized) {
        return XST_FAILURE;
    }

    /* Get standard timing info for the video mode */
    const XVidC_VideoTiming* timing = XVidC_GetTimingInfo(videoMode);
    if (!timing) {
        xil_printf("ERR: No timing info for video mode %d\r\n", videoMode);
        return XST_FAILURE;
    }

    /* Configure timing parameters, adjusted for pixels per clock */
    driver->timing.HActiveVideo = timing->HActive / pixelsPerClk;
    driver->timing.HFrontPorch = timing->HFrontPorch / pixelsPerClk;
    driver->timing.HSyncWidth = timing->HSyncWidth / pixelsPerClk;
    driver->timing.HBackPorch = timing->HBackPorch / pixelsPerClk;
    driver->timing.HSyncPolarity = timing->HSyncPolarity;
    
    driver->timing.VActiveVideo = timing->VActive;
    driver->timing.V0FrontPorch = timing->F0PVFrontPorch;
    driver->timing.V0SyncWidth = timing->F0PVSyncWidth;
    driver->timing.V0BackPorch = timing->F0PVBackPorch;
    driver->timing.VSyncPolarity = timing->VSyncPolarity;

    /* Set generator timing */
    XVtc_SetGeneratorTiming(&driver->instance, &driver->timing);

    return XST_SUCCESS;
}

void VtcDriver_Enable(VtcDriver* driver)
{
    if (driver && driver->isInitialized) {
        XVtc_Enable(&driver->instance);
        XVtc_EnableGenerator(&driver->instance);
        driver->isEnabled = 1;
    }
}

void VtcDriver_Disable(VtcDriver* driver)
{
    if (driver && driver->isInitialized) {
        XVtc_DisableGenerator(&driver->instance);
        XVtc_Disable(&driver->instance);
        driver->isEnabled = 0;
    }
}

void VtcDriver_RegUpdateEnable(VtcDriver* driver)
{
    if (driver && driver->isInitialized) {
        XVtc_RegUpdateEnable(&driver->instance);
    }
}
