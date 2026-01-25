/**
 * @file console.h
 * @brief Console command interface for UART interaction
 */

#ifndef CONSOLE_H
#define CONSOLE_H

#include "xil_types.h"

/**
 * Command handler function type
 * 
 * @param argc Number of arguments
 * @param argv Array of argument strings
 * @return 0 on success, non-zero on failure
 */
typedef int (*CommandHandler)(int argc, char** argv);

/**
 * Command descriptor
 */
typedef struct {
    const char* name;
    const char* help;
    CommandHandler handler;
} ConsoleCommand;

/**
 * Initialize console
 */
void Console_Init(void);

/**
 * Poll console for input (call from main loop)
 * Checks for incoming characters and processes complete commands
 */
void Console_Poll(void);

/**
 * Print console prompt
 */
void Console_PrintPrompt(void);

/**
 * Print help for all commands
 */
void Console_PrintHelp(void);

#endif /* CONSOLE_H */
