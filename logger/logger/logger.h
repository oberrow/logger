#pragma once
#include <stdio.h>
#if _DEBUG
#pragma comment(lib, "C:/Code/logger/x64/Debug/logger.lib")
#else
#pragma comment(lib, "C:/Code/logger/x64/Release/logger.lib")
#endif
typedef enum __loglevel
{
	// Only accepts logging info, warnings, errors, and fatal errors
	LOGGER_level_log,
	// Only accepts logging warnings, errors, and fatal errors
	LOGGER_level_warning,
	// Only accepts logging errors, and fatal errors
	LOGGER_level_error,
	// Only accepts logging fatal errors
	LOGGER_level_fatal,
} LOGGER_loglevel;

typedef enum __logerror
{
	LOGGER_error_none,
	LOGGER_error_level_not_high_enough,
	// check errno
	LOGGER_error_cannot_open_file,
	// check errno
	LOGGER_error_bad_alloc,
	// check errno
	LOGGER_error_general_file_error
} LOGGER_logerror;

typedef struct __logger
{
	LOGGER_loglevel level;
	LOGGER_logerror ec;
	bool autoLN;
	// NEVER change manually or bad stuff will happen
	char* filename;
	// NEVER change manually or bad stuff will happen
	FILE* filehandle;
	bool good;
	bool printTime;
	bool useStderr;
	bool useStdout;
} *LOGGER_logger, **LOGGER_plogger;
#ifdef __cplusplus
extern "C" {
#endif
	LOGGER_logger logger_create(LOGGER_loglevel level);
	LOGGER_logger logger_create_autoln(LOGGER_loglevel level, bool useAutoNewline);
	LOGGER_logger logger_create_autoln_with_file(LOGGER_loglevel level, bool useAutoNewline, const char* filename, bool useStdout, bool useStderr);
	void logger_destroy(LOGGER_plogger logger);
	LOGGER_logerror logger_get_error(LOGGER_logger logger);
	int logger_internal_count(const char* string, char c);
	int logger_log(LOGGER_logger logger, const char* format, ...);
	int logger_log_warning(LOGGER_logger logger, const char* format, ...);
	int logger_log_error(LOGGER_logger logger, const char* format, ...);
	int logger_log_fatal_error(LOGGER_logger logger, const char* format, ...);
#ifdef __cplusplus
}
#endif