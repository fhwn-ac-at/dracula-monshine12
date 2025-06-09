#pragma once

typedef enum {
    ERROR, INFO, DEBUG
} LogLevel;

/**
 * @brief Writes a log message to output screen
 * 
 * @param log_level The log level of the message to be output
 * @param function_name Name of the function which calls this log function
 * @param message Message to be output when called
 */
void logm(LogLevel log_level, char* function_name, char* message);
