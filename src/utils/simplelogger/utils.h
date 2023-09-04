#include <Arduino.h>

#ifndef LOG_APPENDER
#define LOG_APPENDER Serial.print
#endif

#ifndef LOG_FLUSHER
#define LOG_FLUSHER Serial.flush
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
	static char buffer[LOG_SUBSTRING_BUFFER_SIZE + 1];

	uint16_t charsCount = min(n, LOG_SUBSTRING_BUFFER_SIZE);
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

#define LOG_AT_LEVEL(level) 		\
	if (logLevel > level) {			\
		return;						\
	}								\
	va_list argv;					\
	va_start(argv, fmt);			\
	log(level, name, fmt, argv);	\
	va_end(argv);
