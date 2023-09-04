#ifndef SIMPLE_LOGGER_H
#define SIMPLE_LOGGER_H

#ifndef COMPONENTS_DEFAULT_LOG_LEVEL
#define COMPONENTS_DEFAULT_LOG_LEVEL LogLevel::TRACE
#endif

/*
 * A logger that can be configured at runtime and preprocessor time.
 * The latter is important when program and data space is limited.
 *
 * Supports: trace, debug, info, warning, error and off log levels
 *
 * In order to configure the overall appender level, at preprocessor time, set the
 * SIMPLE_LOGGER_APPENDER_LEVEL to the desired one.
 */
#define APPENDER_LEVEL_OFF 6
#define APPENDER_LEVEL_ERROR 5
#define APPENDER_LEVEL_WARNING 4
#define APPENDER_LEVEL_INFO 3
#define APPENDER_LEVEL_DEBUG 2
#define APPENDER_LEVEL_TRACE 1

#ifndef SIMPLE_LOGGER_APPENDER_LEVEL
/* Changing this value will condition the program and data size of the executable.
 * E.g. if the program contains logger.trace("Hello world")
 * but the appender is set to a level > APPENDER_LEVEL_TRACE (DEBUG, INFO, OFF, etc.),
 * the trace's method will be empty and the compiler will perform program and data space usage.
 */
#define SIMPLE_LOGGER_APPENDER_LEVEL APPENDER_LEVEL_TRACE
#endif

#if SIMPLE_LOGGER_APPENDER_LEVEL < APPENDER_LEVEL_OFF
/*
 * You can use this macro to check if log is enabled or not
 * e.g. to set up an appender Serial.begin(...) etc.
 */
#define SIMPLE_LOGGER_APPENDER_ENABLED
#endif

/*
 * Note that the log can be longer than LOG_SUBSTRING_BUFFER_SIZE
 * The buffer is only used to print the substring contained
 * between an %x token and the previous one (or begin of the string)
 */
#ifndef LOG_SUBSTRING_BUFFER_SIZE
#define LOG_SUBSTRING_BUFFER_SIZE 50
#endif

#include "utils.h"

enum class LogLevel {
    TRACE, DEBUG, INFO, WARNING, ERROR, OFF
};

class Logger {
public:
    Logger(const char *name, LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);

    const char *getName() const;

    void setName(const char *instanceName);

    LogLevel getLogLevel() const;

    void setLogLevel(LogLevel logLevel);

    void error(const char *fmt, ...);

    void warn(const char *fmt, ...);

    void info(const char *fmt, ...);

    void debug(const char *fmt, ...);

    void trace(const char *fmt, ...);

private:
    static void log(LogLevel logLevel, const char *name, const char *fmt,
                    va_list argv);

    static const char *getLogLevelDescr(LogLevel level);

    const char *name;
    LogLevel logLevel;
};

inline Logger::Logger(const char *name, LogLevel logLevel) :
        name(name), logLevel(logLevel) {
}

inline const char *Logger::getName() const {
    return name;
}

inline void Logger::setName(const char *name) {
    this->name = name;
}

inline LogLevel Logger::getLogLevel() const {
    return logLevel;
}

inline void Logger::setLogLevel(LogLevel logLevel) {
    this->logLevel = logLevel;
}

inline void Logger::error(const char *fmt, ...) {
#if SIMPLE_LOGGER_APPENDER_LEVEL <= APPENDER_LEVEL_ERROR
    LOG_AT_LEVEL(LogLevel::ERROR);
#endif
}

inline void Logger::warn(const char *fmt, ...) {
#if SIMPLE_LOGGER_APPENDER_LEVEL <= APPENDER_LEVEL_WARNING
    LOG_AT_LEVEL(LogLevel::WARNING);
#endif
}

inline void Logger::info(const char *fmt, ...) {
#if SIMPLE_LOGGER_APPENDER_LEVEL <= APPENDER_LEVEL_INFO
    LOG_AT_LEVEL(LogLevel::INFO);
#endif
}

inline void Logger::debug(const char *fmt, ...) {
#if SIMPLE_LOGGER_APPENDER_LEVEL <= APPENDER_LEVEL_DEBUG
    LOG_AT_LEVEL(LogLevel::DEBUG);
#endif
}

inline void Logger::trace(const char *fmt, ...) {
#if SIMPLE_LOGGER_APPENDER_LEVEL <= APPENDER_LEVEL_TRACE
    LOG_AT_LEVEL(LogLevel::TRACE);
#endif
}

inline void Logger::log(LogLevel logLevel, const char *name, const char *fmt,
                        va_list argv) {
    printDate(millis());
    LOG_APPENDER(" [");
    LOG_APPENDER(getLogLevelDescr(logLevel));
    LOG_APPENDER("]");

    if (name != nullptr) {
        LOG_APPENDER(" ");
        LOG_APPENDER(name);
    }

    LOG_APPENDER(" - ");

    aprintf(fmt, argv);

    LOG_APPENDER('\n');
    LOG_FLUSHER();
}

inline const char *Logger::getLogLevelDescr(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE:
            return "TRACE";
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return " INFO";
        case LogLevel::WARNING:
            return " WARN";
        default:
            return "ERROR";
    }
}

#endif // SIMPLE_LOGGER_H
