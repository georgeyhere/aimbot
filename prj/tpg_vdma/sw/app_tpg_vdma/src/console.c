/**
 * @file console.c
 * @brief Console command interface implementation
 */

#include "console.h"
#include "video_pipeline.h"
#include "video_modes.h"
#include "xil_printf.h"
#include "app_config.h"
#include <string.h>
#include <stdlib.h>

/* Console state */
static char cmdBuffer[CONSOLE_MAX_CMD_LEN];
static int cmdIndex = 0;

/* Forward declarations of command handlers */
static int cmd_help(int argc, char** argv);
static int cmd_init(int argc, char** argv);
static int cmd_mode(int argc, char** argv);
static int cmd_pattern(int argc, char** argv);
static int cmd_start(int argc, char** argv);
static int cmd_stop(int argc, char** argv);
static int cmd_status(int argc, char** argv);
static int cmd_reset(int argc, char** argv);
static int cmd_modes(int argc, char** argv);

/* Command table */
static const ConsoleCommand commands[] = {
    {"help",    "Show this help message", cmd_help},
    {"init",    "Initialize video pipeline", cmd_init},
    {"mode",    "Set video mode (1080p60, 4k30, 4k60)", cmd_mode},
    {"pattern", "Set TPG pattern (0-20)", cmd_pattern},
    {"start",   "Start video pipeline", cmd_start},
    {"stop",    "Stop video pipeline", cmd_stop},
    {"status",  "Show pipeline status", cmd_status},
    {"reset",   "Reset all IP blocks", cmd_reset},
    {"modes",   "List available video modes", cmd_modes},
    {NULL, NULL, NULL}  /* Sentinel */
};

void Console_Init(void)
{
    cmdIndex = 0;
    cmdBuffer[0] = '\0';
    
    xil_printf("\r\n");
    xil_printf("======================================\r\n");
    xil_printf("  Video Pipeline Demo Application\r\n");
    xil_printf("======================================\r\n");
    xil_printf("Type 'help' for available commands\r\n");
    xil_printf("\r\n");
    
    Console_PrintPrompt();
}

void Console_PrintPrompt(void)
{
    xil_printf(CONSOLE_PROMPT);
}

void Console_PrintHelp(void)
{
    xil_printf("Available commands:\r\n");
    for (int i = 0; commands[i].name != NULL; i++) {
        xil_printf("  %-10s - %s\r\n", commands[i].name, commands[i].help);
    }
}

/* Parse command line into argc/argv */
static int parseCommand(char* cmdLine, char** argv, int maxArgs)
{
    int argc = 0;
    char* token = strtok(cmdLine, " \t");
    
    while (token != NULL && argc < maxArgs) {
        argv[argc++] = token;
        token = strtok(NULL, " \t");
    }
    
    return argc;
}

/* Execute a command */
static void executeCommand(char* cmdLine)
{
    char* argv[CONSOLE_MAX_ARGS];
    int argc;
    
    /* Skip empty commands */
    if (strlen(cmdLine) == 0) {
        return;
    }
    
    /* Parse command */
    argc = parseCommand(cmdLine, argv, CONSOLE_MAX_ARGS);
    if (argc == 0) {
        return;
    }
    
    /* Find and execute command */
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(argv[0], commands[i].name) == 0) {
            commands[i].handler(argc, argv);
            return;
        }
    }
    
    /* Command not found */
    xil_printf("Unknown command: %s\r\n", argv[0]);
    xil_printf("Type 'help' for available commands\r\n");
}

void Console_Poll(void)
{
    /* Note: This is a simplified polling implementation
     * In a real system, you'd use XUartLite or XUartPs functions
     * For now, this is a placeholder for the architecture
     */
    
    /* This function would:
     * 1. Check if character available from UART
     * 2. Read character
     * 3. Echo character
     * 4. Add to buffer
     * 5. On newline, execute command and clear buffer
     */
}

/* Command handler implementations */

static int cmd_help(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    
    Console_PrintHelp();
    return 0;
}

static int cmd_init(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    
    int status = VideoPipeline_Init();
    if (status != XST_SUCCESS) {
        xil_printf("Failed to initialize pipeline\r\n");
        return -1;
    }
    
    xil_printf("Pipeline initialized successfully\r\n");
    return 0;
}

static int cmd_mode(int argc, char** argv)
{
    if (argc < 2) {
        xil_printf("Usage: mode <mode_name>\r\n");
        xil_printf("Available modes: 1080p60, 4k30, 4k60\r\n");
        return -1;
    }
    
    int status = VideoPipeline_SetVideoMode(argv[1]);
    if (status != XST_SUCCESS) {
        xil_printf("Failed to set video mode\r\n");
        return -1;
    }
    
    return 0;
}

static int cmd_pattern(int argc, char** argv)
{
    if (argc < 2) {
        xil_printf("Usage: pattern <pattern_id>\r\n");
        xil_printf("Pattern ID: 0-20\r\n");
        xil_printf("  0 = Passthrough\r\n");
        xil_printf("  1 = Horizontal Ramp\r\n");
        xil_printf("  2 = Vertical Ramp\r\n");
        xil_printf("  3 = Temporal Ramp\r\n");
        xil_printf("  4 = Solid Red\r\n");
        xil_printf("  5 = Solid Green\r\n");
        xil_printf("  6 = Solid Blue\r\n");
        xil_printf("  7 = Solid Black\r\n");
        xil_printf("  8 = Solid White\r\n");
        xil_printf("  9 = Color Bars\r\n");
        xil_printf("  10 = Zone Plate\r\n");
        xil_printf("  11 = Tartan Color Bars\r\n");
        xil_printf("  12 = Cross Hatch\r\n");
        xil_printf("  13 = Color Sweep\r\n");
        return -1;
    }
    
    int patternId = atoi(argv[1]);
    
    int status = VideoPipeline_SetPattern(patternId);
    if (status != XST_SUCCESS) {
        xil_printf("Failed to set pattern\r\n");
        return -1;
    }
    
    return 0;
}

static int cmd_start(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    
    int status = VideoPipeline_Start();
    if (status != XST_SUCCESS) {
        xil_printf("Failed to start pipeline\r\n");
        return -1;
    }
    
    return 0;
}

static int cmd_stop(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    
    int status = VideoPipeline_Stop();
    if (status != XST_SUCCESS) {
        xil_printf("Failed to stop pipeline\r\n");
        return -1;
    }
    
    return 0;
}

static int cmd_status(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    
    const char* status = VideoPipeline_GetStatusString();
    xil_printf("%s\r\n", status);
    
    return 0;
}

static int cmd_reset(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    
    VideoPipeline_ResetIp();
    return 0;
}

static int cmd_modes(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    
    VideoMode_PrintAvailable();
    return 0;
}
