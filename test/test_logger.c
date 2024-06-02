#define LOGGER_IMPLEMENTATION
#include "logger.h"

int main(void) {
    LOGGER_LOG(LOGGER_LOG_LEVEL_DEBUG, "Test debug");
    LOGGER_LOG(LOGGER_LOG_LEVEL_INFO, "Test info");
    LOGGER_LOG(LOGGER_LOG_LEVEL_WARNING, "Test warning");
    LOGGER_LOG(LOGGER_LOG_LEVEL_ERROR, "Test error");
    LOGGER_LOG(LOGGER_LOG_LEVEL_FATAL, "Test fatal");
    LOGGER_LOG(LOGGER_LOG_LEVEL_INFO, "Test info %f", 1.f);

    LOGGER_ASSERT(1 == 1);
    LOGGER_ASSERT_MSG(1 == 1, "Should pass");
}
