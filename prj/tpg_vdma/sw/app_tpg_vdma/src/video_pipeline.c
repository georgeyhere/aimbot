/**
 * @file video_pipeline.c
 * @brief Video Pipeline Manager Implementation
 */

#include "video_pipeline.h"
#include "xparameters.h"
#include "xil_printf.h"
#include "sleep.h"
#include "app_config.h"
#include <stdio.h>

/* Singleton instance */
static VideoPipeline g_pipeline = {
    .state = PIPELINE_STATE_UNINITIALIZED
};

VideoPipeline* VideoPipeline_GetInstance(void)
{
    return &g_pipeline;
}

int VideoPipeline_Init(void)
{
    VideoPipeline* pipeline = &g_pipeline;
    int status;

    xil_printf("Initializing video pipeline...\r\n");

    /* Initialize GPIO pointers */
    pipeline->gpioReset = (u32 volatile*)XPAR_HLS_IP_RESET_BASEADDR;
    pipeline->gpioLockMonitor = (u32 volatile*)XPAR_VIDEO_LOCK_MONITOR_BASEADDR;

    /* Initialize VTC */
    status = VtcDriver_Init(&pipeline->vtc, XPAR_XVTC_0_BASEADDR);
    if (status != XST_SUCCESS) {
        xil_printf("ERR: VTC init failed\r\n");
        pipeline->state = PIPELINE_STATE_ERROR;
        return XST_FAILURE;
    }

    /* Initialize source TPG */
    status = TpgDriver_Init(&pipeline->sourceTpg, XPAR_V_TPG_0_BASEADDR);
    if (status != XST_SUCCESS) {
        xil_printf("ERR: Source TPG init failed\r\n");
        pipeline->state = PIPELINE_STATE_ERROR;
        return XST_FAILURE;
    }

    /* Initialize overlay TPG (if present) */
    pipeline->hasOverlayTpg = 0;
#ifdef XPAR_V_TPG_1_BASEADDR
    status = TpgDriver_Init(&pipeline->overlayTpg, XPAR_V_TPG_1_BASEADDR);
    if (status == XST_SUCCESS) {
        pipeline->hasOverlayTpg = 1;
        xil_printf("Overlay TPG detected\r\n");
    }
#endif

    /* Initialize VDMA */
    status = VdmaDriver_Init(&pipeline->vdma, XPAR_AXIVDMA_0_BASEADDR);
    if (status != XST_SUCCESS) {
        xil_printf("ERR: VDMA init failed\r\n");
        pipeline->state = PIPELINE_STATE_ERROR;
        return XST_FAILURE;
    }

    /* Reset IP blocks */
    VideoPipeline_ResetIp();

    /* Set default configuration */
    pipeline->currentMode = DEFAULT_VIDEO_MODE;
    pipeline->currentPattern = DEFAULT_TPG_PATTERN;
    pipeline->state = PIPELINE_STATE_INITIALIZED;

    xil_printf("Video pipeline initialized successfully\r\n");
    return XST_SUCCESS;
}

int VideoPipeline_SetVideoModeById(VideoModeId modeId)
{
    VideoPipeline* pipeline = &g_pipeline;
    int status;

    if (pipeline->state == PIPELINE_STATE_UNINITIALIZED) {
        xil_printf("ERR: Pipeline not initialized\r\n");
        return XST_FAILURE;
    }

    const VideoModeDescriptor* mode = VideoMode_GetDescriptor(modeId);
    if (!mode) {
        xil_printf("ERR: Invalid video mode ID\r\n");
        return XST_FAILURE;
    }

    xil_printf("Setting video mode: %s\r\n", mode->description);

    /* Get pixels per clock from TPG configuration */
    u16 pixelsPerClk = TpgDriver_GetPixelsPerClock(&pipeline->sourceTpg);

    /* Configure clock generator */
    status = ClkGenDriver_ConfigureForMode(&pipeline->clkGen, modeId, pixelsPerClk);
    if (status != XST_SUCCESS) {
        pipeline->state = PIPELINE_STATE_ERROR;
        return XST_FAILURE;
    }

    /* Configure source TPG */
    status = TpgDriver_Configure(&pipeline->sourceTpg, mode->width, mode->height, 
                                 pipeline->currentPattern);
    if (status != XST_SUCCESS) {
        pipeline->state = PIPELINE_STATE_ERROR;
        return XST_FAILURE;
    }

    /* Configure overlay TPG if present */
    if (pipeline->hasOverlayTpg) {
        status = TpgDriver_ConfigurePassthrough(&pipeline->overlayTpg, mode->width, mode->height,
                                               pipeline->currentPattern);
        if (status != XST_SUCCESS) {
            pipeline->state = PIPELINE_STATE_ERROR;
            return XST_FAILURE;
        }
        pixelsPerClk = TpgDriver_GetPixelsPerClock(&pipeline->overlayTpg);
    }

    /* Configure VTC */
    status = VtcDriver_Configure(&pipeline->vtc, mode->xvidc_mode, pixelsPerClk);
    if (status != XST_SUCCESS) {
        pipeline->state = PIPELINE_STATE_ERROR;
        return XST_FAILURE;
    }

    /* Configure VDMA */
    status = VdmaDriver_Configure(&pipeline->vdma, mode->width, mode->height,
                                  VDMA_FRAME_BUFFER_BASE, VDMA_FRAME_COUNT);
    if (status != XST_SUCCESS) {
        pipeline->state = PIPELINE_STATE_ERROR;
        return XST_FAILURE;
    }

    pipeline->currentMode = modeId;
    pipeline->state = PIPELINE_STATE_CONFIGURED;

    xil_printf("Video mode configured successfully\r\n");
    return XST_SUCCESS;
}

int VideoPipeline_SetVideoMode(const char* modeName)
{
    const VideoModeDescriptor* mode = VideoMode_GetDescriptorByName(modeName);
    if (!mode) {
        xil_printf("ERR: Unknown video mode: %s\r\n", modeName);
        return XST_FAILURE;
    }

    return VideoPipeline_SetVideoModeById(mode->id);
}

int VideoPipeline_SetPattern(u32 patternId)
{
    VideoPipeline* pipeline = &g_pipeline;

    if (pipeline->state == PIPELINE_STATE_UNINITIALIZED) {
        xil_printf("ERR: Pipeline not initialized\r\n");
        return XST_FAILURE;
    }

    int status = TpgDriver_SetPattern(&pipeline->sourceTpg, patternId);
    if (status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    if (pipeline->hasOverlayTpg) {
        status = TpgDriver_SetPattern(&pipeline->overlayTpg, patternId);
        if (status != XST_SUCCESS) {
            return XST_FAILURE;
        }
    }

    pipeline->currentPattern = patternId;
    xil_printf("Pattern set to %d\r\n", patternId);

    return XST_SUCCESS;
}

int VideoPipeline_Start(void)
{
    VideoPipeline* pipeline = &g_pipeline;

    if (pipeline->state != PIPELINE_STATE_CONFIGURED && 
        pipeline->state != PIPELINE_STATE_RUNNING) {
        xil_printf("ERR: Pipeline not configured\r\n");
        return XST_FAILURE;
    }

    xil_printf("Starting video pipeline...\r\n");

    /* Start TPG */
    TpgDriver_Start(&pipeline->sourceTpg);
    if (pipeline->hasOverlayTpg) {
        TpgDriver_Start(&pipeline->overlayTpg);
    }

    /* Start VTC */
    VtcDriver_Enable(&pipeline->vtc);
    VtcDriver_RegUpdateEnable(&pipeline->vtc);

    /* Start VDMA */
    int status = VdmaDriver_Start(&pipeline->vdma);
    if (status != XST_SUCCESS) {
        pipeline->state = PIPELINE_STATE_ERROR;
        return XST_FAILURE;
    }

    pipeline->state = PIPELINE_STATE_RUNNING;

    /* Wait a bit and check lock status */
    usleep(IP_RESET_DELAY_US);

    if (VideoPipeline_IsVideoLocked()) {
        xil_printf("Video pipeline started - VIDEO LOCKED\r\n");
    } else {
        xil_printf("WARNING: Video pipeline started but not locked\r\n");
    }

    return XST_SUCCESS;
}

int VideoPipeline_Stop(void)
{
    VideoPipeline* pipeline = &g_pipeline;

    if (pipeline->state == PIPELINE_STATE_UNINITIALIZED) {
        return XST_FAILURE;
    }

    xil_printf("Stopping video pipeline...\r\n");

    /* Stop VDMA */
    VdmaDriver_Stop(&pipeline->vdma);

    /* Stop VTC */
    VtcDriver_Disable(&pipeline->vtc);

    /* Stop TPG */
    TpgDriver_Stop(&pipeline->sourceTpg);
    if (pipeline->hasOverlayTpg) {
        TpgDriver_Stop(&pipeline->overlayTpg);
    }

    if (pipeline->state == PIPELINE_STATE_RUNNING) {
        pipeline->state = PIPELINE_STATE_CONFIGURED;
    }

    xil_printf("Video pipeline stopped\r\n");
    return XST_SUCCESS;
}

void VideoPipeline_ResetIp(void)
{
    VideoPipeline* pipeline = &g_pipeline;

    if (pipeline->gpioReset) {
        xil_printf("Resetting IP blocks...\r\n");
        
        *pipeline->gpioReset = 0;  /* Assert reset */
        usleep(IP_RESET_DELAY_US);
        
        *pipeline->gpioReset = 1;  /* Release reset */
        usleep(IP_RESET_DELAY_US);
        
        xil_printf("IP reset complete\r\n");
    }
}

u8 VideoPipeline_IsVideoLocked(void)
{
    VideoPipeline* pipeline = &g_pipeline;
    
    if (pipeline->gpioLockMonitor) {
        return (*pipeline->gpioLockMonitor) & 0x1;
    }
    
    return 0;
}

PipelineState VideoPipeline_GetState(void)
{
    return g_pipeline.state;
}

const char* VideoPipeline_GetStatusString(void)
{
    static char statusBuf[128];
    VideoPipeline* pipeline = &g_pipeline;

    const VideoModeDescriptor* mode = VideoMode_GetDescriptor(pipeline->currentMode);
    const char* modeName = mode ? mode->name : "unknown";
    
    const char* stateStr;
    switch (pipeline->state) {
        case PIPELINE_STATE_UNINITIALIZED: stateStr = "UNINITIALIZED"; break;
        case PIPELINE_STATE_INITIALIZED:   stateStr = "INITIALIZED"; break;
        case PIPELINE_STATE_CONFIGURED:    stateStr = "CONFIGURED"; break;
        case PIPELINE_STATE_RUNNING:       stateStr = "RUNNING"; break;
        case PIPELINE_STATE_ERROR:         stateStr = "ERROR"; break;
        default:                           stateStr = "UNKNOWN"; break;
    }

    u8 locked = VideoPipeline_IsVideoLocked();

    snprintf(statusBuf, sizeof(statusBuf), 
             "State: %s | Mode: %s | Pattern: %d | Locked: %s",
             stateStr, modeName, pipeline->currentPattern, locked ? "YES" : "NO");

    return statusBuf;
}
