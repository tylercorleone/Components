#ifndef SIMPLELOGGER_H
#define SIMPLELOGGER_H

#define OFF_LEVEL 6
#define ERROR_LEVEL 5
#define WARNING_LEVEL 4
#define INFO_LEVEL 3
#define DEBUG_LEVEL 2
#define TRACE_LEVEL 1

#ifndef LOG_LEVEL
/*
 * The default minimum log level configurable at preprocessor time
 * Changing this value will condition the program and data size of
 * your executable e.g. if it contains "logger.trace("Hello world")
 * and LOG_LEVEL is set to a level > TRACE_LEVEL
 */
#define LOG_LEVEL TRACE_LEVEL
#endif

#if LOG_LEVEL < OFF_LEVEL
/*
 * You can use this macro to check if log is enabled or not
 * e.g. to setup an appender Serial.begin(...) etc.
 */
#define LOG_ENABLED
#endif

/*
 * Note that the can be longer than LOG_SUBSTRING_BUFFER_SIZE
 * The buffer is only used to print the substring contained
 * between a %x token and the previous one (or begin of the string)
 */
#ifndef LOG_SUBSTRING_BUFFER_SIZE
#define LOG_SUBSTRING_BUFFER_SIZE 50
#endif


#ifdef LOG_ENABLED

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

void printDate(uint32_t millis);
void printFirstNCharacters(const char *str, uint16_t n);
void aprintf(const char *str, va_list argv);

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

	uint16_t charsCount = n < (LOG_SUBSTRING_BUFFER_SIZE - 1) ? n : (LOG_SUBSTRING_BUFFER_SIZE - 1);
	strncpy(buffer, str, charsCount);
	buffer[charsCount] = '\0';
	LOG_APPENDER(buffer);

	if (charsCount != n) {
		LOG_APPENDER("...");
	}
}

#include <stdarg.h>

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

#endif // LOG_ENABLED

#define LOG_AT_LEVEL(level) 		\
	if (logLevel > level) {			\
		return;						\
	}								\
	va_list argv;					\
	va_start(argv, fmt);			\
	log(level, name, fmt, argv);	\
	va_end(argv);

#include "Logger.h"

#undef LOG_AT_LEVEL

#endif
