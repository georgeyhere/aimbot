/**
 * @file video_modes.h
 * @brief Video mode definitions and configuration tables
 */

#ifndef VIDEO_MODES_H
#define VIDEO_MODES_H

#include "xvidc.h"

/**
 * Supported video modes for the system
 */
typedef enum {
    VIDEO_MODE_1080P60,
    VIDEO_MODE_4K30,
    VIDEO_MODE_4K60,
    VIDEO_MODE_COUNT
} VideoModeId;

/**
 * Video mode descriptor
 */
typedef struct {
    VideoModeId id;
    XVidC_VideoMode xvidc_mode;
    const char* name;
    const char* description;
    u16 width;
    u16 height;
    u16 fps;
} VideoModeDescriptor;

/**
 * Clock configuration for video modes
 * Indexed by [mode_index][pixels_per_clock_index]
 */
typedef struct {
    u32 divClkDivide;
    u32 clkFbOutMult;
    u32 clkFbOutFrac;
    u32 clkOut0Div[XVIDC_PPC_NUM_SUPPORTED];
    u32 clkOut0Frac[XVIDC_PPC_NUM_SUPPORTED];
} VideoClockConfig;

/**
 * Get video mode descriptor by ID
 */
const VideoModeDescriptor* VideoMode_GetDescriptor(VideoModeId id);

/**
 * Get video mode descriptor by name
 */
const VideoModeDescriptor* VideoMode_GetDescriptorByName(const char* name);

/**
 * Get clock configuration for a video mode
 */
const VideoClockConfig* VideoMode_GetClockConfig(VideoModeId id);

/**
 * Print all available video modes
 */
void VideoMode_PrintAvailable(void);

#endif /* VIDEO_MODES_H */
