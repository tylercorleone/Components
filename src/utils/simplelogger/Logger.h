#ifndef LOGGER_H
#define LOGGER_H

#include "simplelogger.h"

/*
 * Simple Logger is a logger that can be configured at runtime
 * and preprocessor time. The latter is an important advantage
 * when program and data space is limited, for example in
 * embedded systems
 *
 * Supports: trace, debug, info, warning, error and off log levels
 *
 * In order to configure the logger at preprocessor time, set the
 * LOG_LEVEL to the desired one. See simplelogger.h for permitted
 * values and other available configurations.
 */

enum class LogLevel {
	TRACE, DEBUG, INFO, WARNING, ERROR, OFF
};

class Logger {
public:
	Logger();
	Logger(const char *name, LogLevel logLevel = LogLevel::TRACE);
	const char* getName() const;
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
	static const char* getLogLevelDescr(LogLevel level);
	const char *name = nullptr;
	LogLevel logLevel = LogLevel::TRACE;
};

inline Logger::Logger() {

}

inline Logger::Logger(const char *name, LogLevel logLevel) :
		name(name), logLevel(logLevel) {
}

inline const char* Logger::getName() const {
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
#if LOG_LEVEL <= ERROR_LEVEL
	LOG_AT_LEVEL(LogLevel::ERROR);
#endif
}

inline void Logger::warn(const char *fmt, ...) {
#if LOG_LEVEL <= WARNING_LEVEL
	LOG_AT_LEVEL(LogLevel::WARNING);
#endif
}

inline void Logger::info(const char *fmt, ...) {
#if LOG_LEVEL <= INFO_LEVEL
	LOG_AT_LEVEL(LogLevel::INFO);
#endif
}

inline void Logger::debug(const char *fmt, ...) {
#if LOG_LEVEL <= DEBUG_LEVEL
	LOG_AT_LEVEL(LogLevel::DEBUG);
#endif
}

inline void Logger::trace(const char *fmt, ...) {
#if LOG_LEVEL <= TRACE_LEVEL
	LOG_AT_LEVEL(LogLevel::TRACE);
#endif
}

inline void Logger::log(LogLevel logLevel, const char *name, const char *fmt,
		va_list argv) {
#ifdef LOG_ENABLED
	LOG_APPENDER(getLogLevelDescr(logLevel));
	LOG_APPENDER(": (");
	printDate(MILLIS_PROVIDER());

	if (name != nullptr) {
		LOG_APPENDER("|");
		LOG_APPENDER(name);
	}

	LOG_APPENDER(") ");

	aprintf(fmt, argv);

	LOG_APPENDER('\n');
	LOG_FLUSHER();
#endif // LOG_ENABLED
}

inline const char* Logger::getLogLevelDescr(LogLevel level) {
//#ifdef LOG_ENABLED
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
//#else
//	return nullptr;
//#endif // LOG_ENABLED
}

#endif
