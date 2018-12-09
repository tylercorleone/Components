#ifndef SIMPLELOGGER_H
#define SIMPLELOGGER_H

#include "Loggable.h"

/*
 * A logger that can be configured at preprocessor time.
 *
 * Log levels: trace, debug, info, warning, error
 *
 * E.g. #define LOG_LEVEL INFO_LEVEL
 *
 * The default level is info.
 * To completely disable the log set it to OFF_LEVEL
 *
 * Usage:
 * info("Hello world");
 *
 * // will log only it "this" has a not null name and a level >= info
 * infoIfNamed("Some info logged");
 *
 * // will log only it "otherInstance" has a not null name and a level >= info
 * infoIfOtherNamed(otherInstance, "More info logged");
 */

#define ERROR_LEVEL 5
#define WARNING_LEVEL 4
#define INFO_LEVEL 3
#define DEBUG_LEVEL 2
#define TRACE_LEVEL 1
#define OFF_LEVEL -1

#if LOG_LEVEL == ERROR_LEVEL or LOG_LEVEL == WARNING_LEVEL or LOG_LEVEL == INFO_LEVEL or LOG_LEVEL == DEBUG_LEVEL or LOG_LEVEL == TRACE_LEVEL
#define LOG_ENABLED
#elif !defined LOG_LEVEL
// default log level if none is specified
#define LOG_LEVEL INFO_LEVEL
#define LOG_ENABLED
#endif

#if LOG_LEVEL == ERROR_LEVEL or LOG_LEVEL == WARNING_LEVEL or LOG_LEVEL == INFO_LEVEL or LOG_LEVEL == DEBUG_LEVEL or LOG_LEVEL == TRACE_LEVEL
#define error(fmt, ...) logMessage(LogLevel::ERROR, fmt, ##__VA_ARGS__)
#define errorIfNamed(fmt, ...) logLoggableInstance(LogLevel::ERROR, this, fmt, ##__VA_ARGS__)
#define errorIfOtherNamed(other, fmt, ...) logLoggableInstance(LogLevel::ERROR, other, fmt, ##__VA_ARGS__)
#else
#define error(fmt, ...)
#define errorIfNamed(fmt, ...)
#define errorIfOtherNamed(other, fmt, ...)
#endif

#if LOG_LEVEL == WARNING_LEVEL or LOG_LEVEL == INFO_LEVEL or LOG_LEVEL == DEBUG_LEVEL or LOG_LEVEL == TRACE_LEVEL
#define warn(fmt, ...) logMessage(LogLevel::WARNING, fmt, ##__VA_ARGS__)
#define warnIfNamed(fmt, ...) logLoggableInstance(LogLevel::WARNING, this, fmt, ##__VA_ARGS__)
#define warnIfOtherNamed(other, fmt, ...) logLoggableInstance(LogLevel::WARNING, other, fmt, ##__VA_ARGS__)
#else
#define warn(fmt, ...)
#define warnIfNamed(fmt, ...)
#define warnIfOtherNamed(other, fmt, ...)
#endif

#if LOG_LEVEL == INFO_LEVEL or LOG_LEVEL == DEBUG_LEVEL or LOG_LEVEL == TRACE_LEVEL
#define info(fmt, ...) logMessage(LogLevel::INFO, fmt, ##__VA_ARGS__)
#define infoIfNamed(fmt, ...) logLoggableInstance(LogLevel::INFO, this, fmt, ##__VA_ARGS__)
#define infoIfOtherNamed(other, fmt, ...) logLoggableInstance(LogLevel::INFO, other, fmt, ##__VA_ARGS__)
#else
#define info(fmt, ...)
#define infoIfNamed(fmt, ...)
#define infoIfOtherNamed(other, fmt, ...)
#endif

#if LOG_LEVEL == DEBUG_LEVEL or LOG_LEVEL == TRACE_LEVEL
#define debug(fmt, ...) logMessage(LogLevel::DEBUG, fmt, ##__VA_ARGS__)
#define debugIfNamed(fmt, ...) logLoggableInstance(LogLevel::DEBUG, this, fmt, ##__VA_ARGS__)
#define debugIfOtherNamed(other, fmt, ...) logLoggableInstance(LogLevel::DEBUG, other, fmt, ##__VA_ARGS__)
#else
#define debug(fmt, ...)
#define debugIfNamed(fmt, ...)
#define debugIfOtherNamed(other, fmt, ...)
#endif

#if LOG_LEVEL == TRACE_LEVEL
#define trace(fmt, ...) logMessage(LogLevel::TRACE, fmt, ##__VA_ARGS__)
#define traceIfNamed(fmt, ...) logLoggableInstance(LogLevel::TRACE, this, fmt, ##__VA_ARGS__)
#define traceIfOtherNamed(other, fmt, ...) logLoggableInstance(LogLevel::TRACE, other, fmt, ##__VA_ARGS__)
#else
#define trace(fmt, ...)
#define traceIfNamed(fmt, ...)
#define traceIfOtherNamed(other, fmt, ...)
#endif

#ifdef LOG_ENABLED

#ifndef LOG_BUFFER_SIZE
#define LOG_BUFFER_SIZE 50
#endif

#ifndef LOG_APPENDER
#include <Arduino.h>
#define LOG_APPENDER Serial.print
#endif

#ifndef LOG_FLUSHER
#include <Arduino.h>
#define LOG_FLUSHER Serial.flush
#endif

#ifndef MILLIS_PROVIDER
#include <Arduino.h>
#define MILLIS_PROVIDER millis
#endif

#include <stdarg.h>

inline const char* getLogLevelDescr(LogLevel level) {
	switch(level) {
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

inline void printDate(uint32_t millis) {
	uint16_t remainingMillis = millis % 1000;
	uint8_t seconds = (millis / 1000) % 60;
	uint8_t minutes = (millis / ((uint32_t) 1000 * 60)) % 60;
	uint32_t hours = (millis / ((uint32_t) 1000 * 60 * 60));

	LOG_APPENDER(hours);
	LOG_APPENDER(':');
	if (minutes < 10) {
		LOG_APPENDER('0');
	}
	LOG_APPENDER(minutes);
	LOG_APPENDER(':');
	if (seconds < 10) {
		LOG_APPENDER('0');
	}
	LOG_APPENDER(seconds);
	LOG_APPENDER('.');
	if (remainingMillis < 10) {
		LOG_APPENDER("00");
	} else if (remainingMillis < 100) {
		LOG_APPENDER("0");
	}
	LOG_APPENDER(remainingMillis);
}

inline void printFirstNCharacters(const char *str, uint16_t n) {
	static char buffer[LOG_BUFFER_SIZE];

	uint16_t charsCount = n < (LOG_BUFFER_SIZE - 1) ? n : (LOG_BUFFER_SIZE - 1);
	strncpy(buffer, str, charsCount);
	buffer[charsCount] = '\0';
	LOG_APPENDER(buffer);

	if (charsCount != n) {
		LOG_APPENDER("...");
	}
}

// idea taken from https://gist.github.com/EleotleCram/eb586037e2976a8d9884
inline void aprintf(const char *str, va_list argv) {

	int i, j;

	for (i = 0, j = 0; str[i] != '\0'; i++) {
		if (str[i] == '%') {

			printFirstNCharacters(str + j, i - j);

			switch (str[++i]) {
				case 'i':
				case 'd': LOG_APPENDER(va_arg(argv, int));
					break;
				case 'u': LOG_APPENDER(va_arg(argv, unsigned));
					break;
				case 'l': LOG_APPENDER(va_arg(argv, long));
					break;
				case 'f': LOG_APPENDER(va_arg(argv, double));
					break;
				case 'c': LOG_APPENDER((char) va_arg(argv, int));
					break;
				case 's': LOG_APPENDER(va_arg(argv, const char *));
					break;
				case '%': LOG_APPENDER('%');
					break;
				default:;
			};

			j = i + 1;
		}
	}

	if (i > j) {
		printFirstNCharacters(str + j, i - j);
	}
}

inline void logMessage(LogLevel level, const char *fmt, ...) {
	va_list argv;
	va_start(argv, fmt);

	LOG_APPENDER(getLogLevelDescr(level));
	LOG_APPENDER(": (");
	printDate(MILLIS_PROVIDER());
	LOG_APPENDER(") ");
	aprintf(fmt, argv);

	va_end(argv);

	LOG_APPENDER('\n');
	LOG_FLUSHER();
}

inline void logLoggableInstance(LogLevel level, Loggable *instance, const char *fmt, ...) {
	if (instance->getName() == nullptr || instance->getLogLevel() > level) {
		return;
	}

	va_list argv;
	va_start(argv, fmt);

	LOG_APPENDER(getLogLevelDescr(level));
	LOG_APPENDER(": (");
	printDate(MILLIS_PROVIDER());
	LOG_APPENDER("|");
	LOG_APPENDER(instance->getName());
	LOG_APPENDER(") ");
	aprintf(fmt, argv);

	va_end(argv);

	LOG_APPENDER('\n');
	LOG_FLUSHER();
}

#endif // LOG_ENABLED

#endif
