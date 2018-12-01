#ifndef PRINTUTILS_H
#define PRINTUTILS_H

#include "commons/Named.h"

#include <Arduino.h>
#include <stdarg.h>

#ifdef INFO
#define info(fmt, ...) logMessage(fmt, ##__VA_ARGS__)
#define infoIfNamed(fmt, ...) logNamedInstance(this, fmt, ##__VA_ARGS__)
#define infoIfOtherNamed(other, fmt, ...) logNamedInstance(other, fmt, ##__VA_ARGS__)
#ifdef DEBUG
#define debug(fmt, ...) logMessage(fmt, ##__VA_ARGS__)
#define debugIfNamed(fmt, ...) logNamedInstance(this, fmt, ##__VA_ARGS__)
#define debugIfOtherNamed(other, fmt, ...) logNamedInstance(other, fmt, ##__VA_ARGS__)
#ifdef TRACE
#define trace(fmt, ...) logMessage(fmt, ##__VA_ARGS__)
#define traceIfNamed(fmt, ...) logNamedInstance(this, fmt, ##__VA_ARGS__)
#define traceIfOtherNamed(other, fmt, ...) logNamedInstance(other, fmt, ##__VA_ARGS__)
#else
#define trace(fmt, ...)
#define traceIfNamed(fmt, ...)
#define traceIfOtherNamed(other, fmt, ...)
#endif
#else
#define debug(fmt, ...)
#define debugIfNamed(fmt, ...)
#define debugIfOtherNamed(other, fmt, ...)
#define trace(fmt, ...)
#define traceIfNamed(fmt, ...)
#define traceIfOtherNamed(other, fmt, ...)
#endif
#else
#define info(fmt, ...)
#define infoIfNamed(fmt, ...)
#define infoIfOtherNamed(other, fmt, ...)
#define trace(fmt, ...)
#define traceIfNamed(fmt, ...)
#define traceIfOtherNamed(other, fmt, ...)
#define debug(fmt, ...)
#define debugIfNamed(fmt, ...)
#define debugIfOtherNamed(other, fmt, ...)
#endif

// idea taken from https://gist.github.com/EleotleCram/eb586037e2976a8d9884
inline void aprintf(const char *str, va_list argv) {
	int i, j;

	for (i = 0, j = 0; str[i] != '\0'; i++) {
		if (str[i] == '%') {

			Serial.write(reinterpret_cast<const uint8_t*>(str + j), i - j);

			switch (str[++i]) {
				case 'i':
				case 'd': Serial.print(va_arg(argv, int));
					break;
				case 'u': Serial.print(va_arg(argv, unsigned));
					break;
				case 'l': Serial.print(va_arg(argv, long));
					break;
				case 'f': Serial.print(va_arg(argv, double));
					break;
				case 'c': Serial.print((char) va_arg(argv, int));
					break;
				case 's': Serial.print(va_arg(argv, const char *));
					break;
				case '%': Serial.print('%');
					break;
				default:;
			};

			j = i + 1;
		}
	}

	if (i > j) {
		Serial.write(reinterpret_cast<const uint8_t*>(str + j), i - j);
	}
}

inline void logMessage(const char *fmt, ...) {
	va_list argv;
	va_start(argv, fmt);

	aprintf(fmt, argv);

	va_end(argv);

	Serial.println();
	Serial.flush();
}

inline void logNamedInstance(Named *instance, const char *fmt, ...) {
	if (instance->getName() == nullptr) {
		return;
	}

	va_list argv;
	va_start(argv, fmt);

	Serial.print(instance->getName());
	Serial.print(": ");
	aprintf(fmt, argv);

	va_end(argv);

	Serial.println();
	Serial.flush();
}

#endif
