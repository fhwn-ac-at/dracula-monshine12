#include "logger.h"
#include <stdio.h>

#define RED "\033[1;31m"  // ERROR color
#define BLUE "\033[1;34m" // DEBUG color
#define RESET "\033[0m"   // Reset color
#define DEBUG_MODE 1

void logm(LogLevel log_level, char* function_name, char* message) {
    switch (log_level) {
        case INFO:
            printf("[INFO] - %s() : %s\n", function_name, message);
            break;
        case DEBUG:
            if (DEBUG_MODE) {
                printf(BLUE "[DEBUG] - %s() : %s\n" RESET, function_name, message);
            }
            break;
        case ERROR:
            printf(RED "[ERROR] - %s() : %s\n" RESET, function_name, message);
            break;
        default:
            logm(ERROR, "log", "Invalid log level!");
            break;
    }
}