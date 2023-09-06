#include "Logger.h"
#include <Arduino.h> // TODO

void printDate(uint32_t millis);
void printFirstNCharacters(const char *str, uint16_t n);
void aprintf(const char *str, va_list argv);

#define LOG_AT_LEVEL(level) 		\
	if (logLevel > level) {			\
		return;						\
	}								\
	va_list argv;					\
	va_start(argv, fmt);			\
	log(level, name, fmt, argv);	\
	va_end(argv);


inline Logger::Logger(const char *name, LogLevel logLevel) :
        name(name), logLevel(logLevel) {
}

inline const char *Logger::getName() const {
    return name;
}

inline void Logger::setName(const char *_name) {
    this->name = _name;
}

inline LogLevel Logger::getLogLevel() const {
    return logLevel;
}

inline void Logger::setLogLevel(LogLevel _logLevel) {
    this->logLevel = _logLevel;
}

inline void Logger::error(const char *fmt, ...) {
#if SIMPLE_LOGGER_MIN_APPENDER_LEVEL <= APPENDER_LEVEL_ERROR
    LOG_AT_LEVEL(LogLevel::ERROR);
#endif
}

inline void Logger::warn(const char *fmt, ...) {
#if SIMPLE_LOGGER_MIN_APPENDER_LEVEL <= APPENDER_LEVEL_WARNING
    LOG_AT_LEVEL(LogLevel::WARNING);
#endif
}

inline void Logger::info(const char *fmt, ...) {
#if SIMPLE_LOGGER_MIN_APPENDER_LEVEL <= APPENDER_LEVEL_INFO
    LOG_AT_LEVEL(LogLevel::INFO);
#endif
}

inline void Logger::debug(const char *fmt, ...) {
#if SIMPLE_LOGGER_MIN_APPENDER_LEVEL <= APPENDER_LEVEL_DEBUG
    LOG_AT_LEVEL(LogLevel::DEBUG);
#endif
}

inline void Logger::trace(const char *fmt, ...) {
#if SIMPLE_LOGGER_MIN_APPENDER_LEVEL <= APPENDER_LEVEL_TRACE
    LOG_AT_LEVEL(LogLevel::TRACE);
#endif
}

inline void Logger::log(LogLevel logLevel, const char *name, const char *fmt,
                        va_list argv) {
#ifdef SIMPLE_LOGGER_APPENDER_ENABLED
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
#endif
}

inline const char *Logger::getLogLevelDescr(LogLevel level) {
#ifdef SIMPLE_LOGGER_APPENDER_ENABLED
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
#else
    return nullptr;
#endif
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
    static char buffer[LOG_SUBSTRING_BUFFER_SIZE + 1];

    uint16_t charsCount = n < LOG_SUBSTRING_BUFFER_SIZE ? n : LOG_SUBSTRING_BUFFER_SIZE;
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
