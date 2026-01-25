/**
 * @file video_pipeline.h
 * @brief Video Pipeline Manager - Singleton coordinator for video subsystem
 */

#ifndef VIDEO_PIPELINE_H
#define VIDEO_PIPELINE_H

#include "tpg_driver.h"
#include "vdma_driver.h"
#include "vtc_driver.h"
#include "video_modes.h"

/**
 * Video pipeline state
 */
typedef enum {
    PIPELINE_STATE_UNINITIALIZED,
    PIPELINE_STATE_INITIALIZED,
    PIPELINE_STATE_CONFIGURED,
    PIPELINE_STATE_RUNNING,
    PIPELINE_STATE_ERROR
} PipelineState;

/**
 * Video pipeline instance (singleton)
 */
typedef struct {
    /* Hardware drivers */
    TpgDriver sourceTpg;
    TpgDriver overlayTpg;
    VdmaDriver vdma;
    VtcDriver vtc;
    ClkGenDriver clkGen;
    
    /* GPIO pointers */
    u32 volatile* gpioReset;
    u32 volatile* gpioLockMonitor;
    
    /* Current configuration */
    VideoModeId currentMode;
    u32 currentPattern;
    
    /* State */
    PipelineState state;
    u8 hasOverlayTpg;
} VideoPipeline;

/**
 * Get singleton instance of video pipeline
 * 
 * @return Pointer to video pipeline instance
 */
VideoPipeline* VideoPipeline_GetInstance(void);

/**
 * Initialize video pipeline (must be called first)
 * 
 * @return XST_SUCCESS or XST_FAILURE
 */
int VideoPipeline_Init(void);

/**
 * Set video mode (resolution and frame rate)
 * 
 * @param modeName Name of video mode ("1080p60", "4k30", "4k60")
 * @return XST_SUCCESS or XST_FAILURE
 */
int VideoPipeline_SetVideoMode(const char* modeName);

/**
 * Set video mode by ID
 * 
 * @param modeId Video mode ID
 * @return XST_SUCCESS or XST_FAILURE
 */
int VideoPipeline_SetVideoModeById(VideoModeId modeId);

/**
 * Set TPG pattern
 * 
 * @param patternId Pattern ID (0-20)
 * @return XST_SUCCESS or XST_FAILURE
 */
int VideoPipeline_SetPattern(u32 patternId);

/**
 * Start video pipeline
 * 
 * @return XST_SUCCESS or XST_FAILURE
 */
int VideoPipeline_Start(void);

/**
 * Stop video pipeline
 * 
 * @return XST_SUCCESS or XST_FAILURE
 */
int VideoPipeline_Stop(void);

/**
 * Reset all IP blocks
 */
void VideoPipeline_ResetIp(void);

/**
 * Check if video is locked
 * 
 * @return 1 if locked, 0 if not locked
 */
u8 VideoPipeline_IsVideoLocked(void);

/**
 * Get current pipeline state
 * 
 * @return Current pipeline state
 */
PipelineState VideoPipeline_GetState(void);

/**
 * Get status string for display
 * 
 * @return Pointer to status string
 */
const char* VideoPipeline_GetStatusString(void);

#endif /* VIDEO_PIPELINE_H */
