/**
 * @file video_modes.c
 * @brief Video mode configuration implementation
 */

#include "video_modes.h"
#include "xil_printf.h"
#include <string.h>

/* Video mode descriptors table */
static const VideoModeDescriptor video_modes[VIDEO_MODE_COUNT] = {
    {
        .id = VIDEO_MODE_1080P60,
        .xvidc_mode = XVIDC_VM_1080_60_P,
        .name = "1080p60",
        .description = "1920x1080 @ 60Hz Progressive",
        .width = 1920,
        .height = 1080,
        .fps = 60
    },
    {
        .id = VIDEO_MODE_4K30,
        .xvidc_mode = XVIDC_VM_UHD_30_P,
        .name = "4k30",
        .description = "3840x2160 @ 30Hz Progressive",
        .width = 3840,
        .height = 2160,
        .fps = 30
    },
    {
        .id = VIDEO_MODE_4K60,
        .xvidc_mode = XVIDC_VM_UHD_60_P,
        .name = "4k60",
        .description = "3840x2160 @ 60Hz Progressive",
        .width = 3840,
        .height = 2160,
        .fps = 60
    }
};

/* Clock configuration table */
static const VideoClockConfig clock_configs[VIDEO_MODE_COUNT] = {
    /* 1080p60 */
    {
        .divClkDivide = 4,
        .clkFbOutMult = 37,
        .clkFbOutFrac = 125,
        .clkOut0Div = {6, 12, 25, 50},
        .clkOut0Frac = {250, 500, 0, 0}
    },
    /* 4K30 */
    {
        .divClkDivide = 4,
        .clkFbOutMult = 37,
        .clkFbOutFrac = 125,
        .clkOut0Div = {3, 6, 12, 25},
        .clkOut0Frac = {125, 250, 500, 0}
    },
    /* 4K60 */
    {
        .divClkDivide = 4,
        .clkFbOutMult = 37,
        .clkFbOutFrac = 125,
        .clkOut0Div = {0, 3, 6, 12},
        .clkOut0Frac = {0, 125, 250, 500}
    }
};

const VideoModeDescriptor* VideoMode_GetDescriptor(VideoModeId id)
{
    if (id >= VIDEO_MODE_COUNT) {
        return NULL;
    }
    return &video_modes[id];
}

const VideoModeDescriptor* VideoMode_GetDescriptorByName(const char* name)
{
    for (int i = 0; i < VIDEO_MODE_COUNT; i++) {
        if (strcmp(video_modes[i].name, name) == 0) {
            return &video_modes[i];
        }
    }
    return NULL;
}

const VideoClockConfig* VideoMode_GetClockConfig(VideoModeId id)
{
    if (id >= VIDEO_MODE_COUNT) {
        return NULL;
    }
    return &clock_configs[id];
}

void VideoMode_PrintAvailable(void)
{
    xil_printf("Available video modes:\r\n");
    for (int i = 0; i < VIDEO_MODE_COUNT; i++) {
        xil_printf("  %s - %s\r\n", 
                   video_modes[i].name, 
                   video_modes[i].description);
    }
}
