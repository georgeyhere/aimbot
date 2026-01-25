/**
 * @file main.c
 * @brief Video Pipeline Demo Application - Main Entry Point
 * 
 * This application demonstrates the use of TPG, VDMA, and VTC IP blocks
 * to generate test patterns and output video. Users can control video
 * modes and patterns via UART console.
 */

#include "xparameters.h"
#include "xil_printf.h"
#include "sleep.h"

#include "video_pipeline.h"
#include "console.h"
#include "app_config.h"

/**
 * Main application entry point
 */
int main(void) 
{
    int status;

    xil_printf("\r\n");
    xil_printf("========================================\r\n");
    xil_printf(" Video Pipeline Demo Application\r\n");
    xil_printf(" Build Date: %s %s\r\n", __DATE__, __TIME__);
    xil_printf("========================================\r\n");
    xil_printf("\r\n");

    /* Initialize console */
    Console_Init();

    /* Initialize video pipeline */
    xil_printf("Initializing video pipeline...\r\n");
    status = VideoPipeline_Init();
    if (status != XST_SUCCESS) {
        xil_printf("FATAL: Video pipeline initialization failed!\r\n");
        return XST_FAILURE;
    }

    /* Set default video mode */
    xil_printf("Setting default video mode...\r\n");
    status = VideoPipeline_SetVideoModeById(DEFAULT_VIDEO_MODE);
    if (status != XST_SUCCESS) {
        xil_printf("WARNING: Failed to set default video mode\r\n");
    }

    /* Start video pipeline */
    xil_printf("Starting video pipeline...\r\n");
    status = VideoPipeline_Start();
    if (status != XST_SUCCESS) {
        xil_printf("WARNING: Failed to start video pipeline\r\n");
    }

    xil_printf("\r\n");
    xil_printf("System ready! Type 'help' for commands.\r\n");
    xil_printf("\r\n");

    /* Main loop */
    while (1) {
        /* Poll for console input */
        Console_Poll();
        
        /* Small delay to prevent tight loop */
        usleep(1000);
    }

    return 0;
}
