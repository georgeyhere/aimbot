/**
 * @file app_config.h
 * @brief Application-level configuration constants
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "xparameters.h"

/* Default video mode on startup */
#define DEFAULT_VIDEO_MODE          VIDEO_MODE_1080P60

/* Default TPG pattern on startup */
#define DEFAULT_TPG_PATTERN         XTPG_BKGND_COLOR_BARS

/* VDMA configuration */
#define VDMA_FRAME_BUFFER_BASE      0x10000000
#define VDMA_FRAME_COUNT            3
#define VDMA_MAX_WIDTH              3840
#define VDMA_MAX_HEIGHT             2160

/* Clock wizard timeouts */
#define CLKGEN_LOCK_TIMEOUT_US      300000
#define CLKGEN_LOCK_RETRY_TIMEOUT   100000

/* Reset delays */
#define IP_RESET_DELAY_US           300000

/* Console configuration */
#define CONSOLE_PROMPT              "video> "
#define CONSOLE_MAX_CMD_LEN         64
#define CONSOLE_MAX_ARGS            8

/* TPG pattern IDs - from xv_tpg.h */
#define TPG_PATTERN_COUNT           21

/* Debug/logging levels */
#define LOG_LEVEL_ERROR             0
#define LOG_LEVEL_WARN              1
#define LOG_LEVEL_INFO              2
#define LOG_LEVEL_DEBUG             3

#ifndef LOG_LEVEL
#define LOG_LEVEL                   LOG_LEVEL_INFO
#endif

#endif /* APP_CONFIG_H */
