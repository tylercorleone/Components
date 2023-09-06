#ifndef SIMPLE_LOGGER_H
#define SIMPLE_LOGGER_H

#include <stdarg.h>

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
 * SIMPLE_LOGGER_MIN_APPENDER_LEVEL to the desired one.
 */
#define APPENDER_LEVEL_OFF 6
#define APPENDER_LEVEL_ERROR 5
#define APPENDER_LEVEL_WARNING 4
#define APPENDER_LEVEL_INFO 3
#define APPENDER_LEVEL_DEBUG 2
#define APPENDER_LEVEL_TRACE 1

#ifndef SIMPLE_LOGGER_MIN_APPENDER_LEVEL
/* Changing this value will condition the program and data size of the executable.
 * E.g. if the program contains logger.trace("Hello world")
 * but the min is set to a higher level  (DEBUG, INFO, OFF, etc.),
 * the trace's method will be empty and the compiler will perform program and data space usage.
 */
#define SIMPLE_LOGGER_MIN_APPENDER_LEVEL APPENDER_LEVEL_TRACE
#endif

#if SIMPLE_LOGGER_MIN_APPENDER_LEVEL < APPENDER_LEVEL_OFF
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

#ifndef LOG_APPENDER
#define LOG_APPENDER Serial.print
#endif

#ifndef LOG_FLUSHER
#define LOG_FLUSHER Serial.flush
#endif

enum class LogLevel {
    TRACE, DEBUG, INFO, WARNING, ERROR, OFF
};

class Logger {
public:
    explicit Logger(const char *name, LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);

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

#endif // SIMPLE_LOGGER_H
