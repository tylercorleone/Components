#ifndef SIMPLE_LOGGER_H
#define SIMPLE_LOGGER_H

#ifndef COMPONENTS_DEFAULT_LOG_LEVEL
#define COMPONENTS_DEFAULT_LOG_LEVEL LogLevel::TRACE
#endif

/*
 * Simple Logger is a logger that can be configured at runtime
 * and preprocessor time. The latter is an important advantage
 * when program and data space is limited, for example in
 * embedded systems
 *
 * Supports: trace, debug, info, warning, error and off log levels
 *
 * In order to configure the logger at preprocessor time, set the
 * SIMPLE_LOGGER_LOG_LEVEL to the desired one.
 */
#define OFF_LEVEL 6
#define ERROR_LEVEL 5
#define WARNING_LEVEL 4
#define INFO_LEVEL 3
#define DEBUG_LEVEL 2
#define TRACE_LEVEL 1

#ifndef SIMPLE_LOGGER_LOG_LEVEL
/*
 * The default minimum log level configurable at preprocessor time
 * Changing this value will condition the program and data size of
 * your executable e.g. if it contains "logger.trace("Hello world")
 * and SIMPLE_LOGGER_LOG_LEVEL is set to a level > TRACE_LEVEL
 */
#define SIMPLE_LOGGER_LOG_LEVEL TRACE_LEVEL
#endif

#if SIMPLE_LOGGER_LOG_LEVEL < OFF_LEVEL
/*
 * You can use this macro to check if log is enabled or not
 * e.g. to set up an appender Serial.begin(...) etc.
 */
#define LOG_ENABLED
#endif

/*
 * Note that the log can be longer than LOG_SUBSTRING_BUFFER_SIZE
 * The buffer is only used to print the substring contained
 * between an %x token and the previous one (or begin of the string)
 */
#ifndef LOG_SUBSTRING_BUFFER_SIZE
#define LOG_SUBSTRING_BUFFER_SIZE 50
#endif

#ifdef LOG_ENABLED

#include "utils.h"

enum class LogLevel {
    TRACE, DEBUG, INFO, WARNING, ERROR, OFF
};

class Logger {
public:
    Logger(const char *name, LogLevel logLevel = LogLevel::TRACE);

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
#if SIMPLE_LOGGER_LOG_LEVEL <= ERROR_LEVEL
    LOG_AT_LEVEL(LogLevel::ERROR);
#endif
}

inline void Logger::warn(const char *fmt, ...) {
#if SIMPLE_LOGGER_LOG_LEVEL <= WARNING_LEVEL
    LOG_AT_LEVEL(LogLevel::WARNING);
#endif
}

inline void Logger::info(const char *fmt, ...) {
#if SIMPLE_LOGGER_LOG_LEVEL <= INFO_LEVEL
    LOG_AT_LEVEL(LogLevel::INFO);
#endif
}

inline void Logger::debug(const char *fmt, ...) {
#if SIMPLE_LOGGER_LOG_LEVEL <= DEBUG_LEVEL
    LOG_AT_LEVEL(LogLevel::DEBUG);
#endif
}

inline void Logger::trace(const char *fmt, ...) {
#if SIMPLE_LOGGER_LOG_LEVEL <= TRACE_LEVEL
    LOG_AT_LEVEL(LogLevel::TRACE);
#endif
}

inline void Logger::log(LogLevel logLevel, const char *name, const char *fmt,
                        va_list argv) {
    LOG_APPENDER(getLogLevelDescr(logLevel));
    LOG_APPENDER(": (");
    printDate(millis());

    if (name != nullptr) {
        LOG_APPENDER("|");
        LOG_APPENDER(name);
    }

    LOG_APPENDER(") ");

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

#endif // LOG_ENABLED

#endif // SIMPLE_LOGGER_H
