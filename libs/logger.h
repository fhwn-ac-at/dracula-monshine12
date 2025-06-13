#pragma once

typedef enum {
    ERROR, INFO, DEBUG
} LogLevel;

/**
 * @brief Logs a message to the console with colored output based on log level.
 *
 * Prints a formatted message to the standard output or error stream, including function name and the log message.
 * Colored output is used to distinguish between `INFO`, `DEBUG`, and `ERROR` levels. `DEBUG` logs are only printed
 * if `DEBUG_MODE` is set to 1.
 *
 * @param log_level The severity level of the message (`INFO`, `DEBUG`, or `ERROR`).
 * @param function_name The name of the function where the log is generated.
 * @param message The log message to be displayed.
 *
 * @note Uses ANSI escape codes for coloring; may not work properly on all terminals.
 * @note If an invalid log level is passed, it recursively logs an error about the invalid level.
 */
void logm(LogLevel log_level, char* function_name, char* message);

