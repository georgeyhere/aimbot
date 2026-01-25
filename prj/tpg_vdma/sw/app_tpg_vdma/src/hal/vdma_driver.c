/**
 * @file vdma_driver.c
 * @brief VDMA Hardware Abstraction Layer Implementation
 */

#include "vdma_driver.h"
#include "xil_printf.h"
#include "app_config.h"

int VdmaDriver_Init(VdmaDriver* driver, UINTPTR baseAddr)
{
    if (!driver) {
        return XST_FAILURE;
    }

    /* Lookup configuration */
    driver->config = XAxiVdma_LookupConfig(baseAddr);
    if (!driver->config) {
        xil_printf("ERR: VDMA configuration not found for base 0x%08X\r\n", (u32)baseAddr);
        return XST_DEVICE_NOT_FOUND;
    }

    /* Initialize the driver */
    int status = XAxiVdma_CfgInitialize(&driver->instance, driver->config, driver->config->BaseAddress);
    if (status != XST_SUCCESS) {
        xil_printf("ERR: VDMA initialization failed: %d\r\n", status);
        return XST_FAILURE;
    }

    driver->isInitialized = 1;
    driver->isRunning = 0;
    driver->frameCount = 0;
    driver->width = 0;
    driver->height = 0;
    driver->stride = 0;
    driver->frameBufferAddr = 0;

    return XST_SUCCESS;
}

int VdmaDriver_Configure(VdmaDriver* driver, u16 width, u16 height, 
                         u32 frameBufferAddr, u8 frameCount)
{
    if (!driver || !driver->isInitialized) {
        return XST_FAILURE;
    }

    if (frameCount < 2 || frameCount > 3) {
        xil_printf("ERR: Invalid frame count %d (must be 2 or 3)\r\n", frameCount);
        return XST_FAILURE;
    }

    /* Calculate stride (bytes per line, assuming 3 bytes per pixel RGB) */
    driver->stride = width * 3;
    driver->width = width;
    driver->height = height;
    driver->frameCount = frameCount;
    driver->frameBufferAddr = frameBufferAddr;

    /* Configure read channel (MM2S) */
    driver->readCfg.VertSizeInput = height;
    driver->readCfg.HoriSizeInput = driver->stride;
    driver->readCfg.Stride = driver->stride;
    driver->readCfg.FrameDelay = 0;
    driver->readCfg.EnableCircularBuf = 1;
    driver->readCfg.EnableSync = 0;
    driver->readCfg.PointNum = 0;
    driver->readCfg.EnableFrameCounter = 0;
    driver->readCfg.FixedFrameStoreAddr = 0;

    /* Configure write channel (S2MM) */
    driver->writeCfg.VertSizeInput = height;
    driver->writeCfg.HoriSizeInput = driver->stride;
    driver->writeCfg.Stride = driver->stride;
    driver->writeCfg.FrameDelay = 0;
    driver->writeCfg.EnableCircularBuf = 1;
    driver->writeCfg.EnableSync = 0;
    driver->writeCfg.PointNum = 0;
    driver->writeCfg.EnableFrameCounter = 0;
    driver->writeCfg.FixedFrameStoreAddr = 0;

    /* Configure frame buffers */
    u32 frameSize = driver->stride * height;
    
    for (int i = 0; i < frameCount; i++) {
        driver->readCfg.FrameStoreStartAddr[i] = frameBufferAddr + (i * frameSize);
        driver->writeCfg.FrameStoreStartAddr[i] = frameBufferAddr + (i * frameSize);
    }

    /* Apply DMA configuration */
    int status = XAxiVdma_DmaConfig(&driver->instance, XAXIVDMA_READ, &driver->readCfg);
    if (status != XST_SUCCESS) {
        xil_printf("ERR: VDMA read channel config failed: %d\r\n", status);
        return XST_FAILURE;
    }

    status = XAxiVdma_DmaConfig(&driver->instance, XAXIVDMA_WRITE, &driver->writeCfg);
    if (status != XST_SUCCESS) {
        xil_printf("ERR: VDMA write channel config failed: %d\r\n", status);
        return XST_FAILURE;
    }

    /* Set frame buffer count */
    status = XAxiVdma_SetFrmStore(&driver->instance, frameCount, XAXIVDMA_READ);
    if (status != XST_SUCCESS) {
        xil_printf("ERR: VDMA set read frame store failed: %d\r\n", status);
        return XST_FAILURE;
    }

    status = XAxiVdma_SetFrmStore(&driver->instance, frameCount, XAXIVDMA_WRITE);
    if (status != XST_SUCCESS) {
        xil_printf("ERR: VDMA set write frame store failed: %d\r\n", status);
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

int VdmaDriver_Start(VdmaDriver* driver)
{
    if (!driver || !driver->isInitialized) {
        return XST_FAILURE;
    }

    /* Start read channel */
    int status = XAxiVdma_DmaStart(&driver->instance, XAXIVDMA_READ);
    if (status != XST_SUCCESS) {
        xil_printf("ERR: VDMA read channel start failed: %d\r\n", status);
        return XST_FAILURE;
    }

    /* Start write channel */
    status = XAxiVdma_DmaStart(&driver->instance, XAXIVDMA_WRITE);
    if (status != XST_SUCCESS) {
        xil_printf("ERR: VDMA write channel start failed: %d\r\n", status);
        return XST_FAILURE;
    }

    driver->isRunning = 1;
    return XST_SUCCESS;
}

int VdmaDriver_Stop(VdmaDriver* driver)
{
    if (!driver || !driver->isInitialized) {
        return XST_FAILURE;
    }

    /* Stop both channels */
    XAxiVdma_DmaStop(&driver->instance, XAXIVDMA_READ);
    XAxiVdma_DmaStop(&driver->instance, XAXIVDMA_WRITE);

    driver->isRunning = 0;
    return XST_SUCCESS;
}

void VdmaDriver_Reset(VdmaDriver* driver)
{
    if (driver && driver->isInitialized) {
        XAxiVdma_Reset(&driver->instance, XAXIVDMA_READ);
        XAxiVdma_Reset(&driver->instance, XAXIVDMA_WRITE);
        driver->isRunning = 0;
    }
}
