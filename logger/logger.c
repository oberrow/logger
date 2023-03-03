// logger.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"

#ifndef _WIN32
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#endif

#include "cross_platform_def.h"

typedef enum __colors
{
	LOGGER_colors_black,
	LOGGER_colors_blue,
	LOGGER_colors_green,
	LOGGER_colors_cyan,
	LOGGER_colors_red,
	LOGGER_colors_magenta,
	LOGGER_colors_brown,
	LOGGER_colors_lightgray,
	LOGGER_colors_darkgray,
	LOGGER_colors_lightblue,
	LOGGER_colors_lightgreen,
	LOGGER_colors_lightcyan,
	LOGGER_colors_lightred,
	LOGGER_colors_lightmagenta,
	LOGGER_colors_yellow,
	LOGGER_colors_white
} LOGGER_colors;


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
	LOGGER_error_cannot_open_file,
	LOGGER_error_bad_alloc,
	LOGGER_error_general_file_error
} LOGGER_logerror;

typedef struct __logger
{
	LOGGER_loglevel level;
	LOGGER_logerror ec;
	bool autoLN;
	char* filename;
	FILE* filehandle;
	bool good;
	bool printTime;
	bool useStderr;
	bool useStdout;
} *LOGGER_logger, **LOGGER_plogger;
LOGGER_logger logger_create(LOGGER_loglevel level)
{
	LOGGER_logger ret = calloc(sizeof(struct __logger), sizeof(struct __logger));
	if (ret == NULL)
		return NULL;
	ret->level = level;
	ret->autoLN = true;
	ret->filename = NULL;
	ret->useStdout = true;
	ret->useStderr = true;
	ret->printTime = true;
	return ret;
}
LOGGER_logger logger_create_autoln(LOGGER_loglevel level, bool useAutoNewline)
{
	LOGGER_logger ret = calloc(sizeof(struct __logger), sizeof(struct __logger));
	if (ret == NULL)
		return NULL;
	ret->level = level;
	ret->autoLN = useAutoNewline;
	ret->filename = NULL;
	ret->filehandle = NULL;
	ret->useStdout = true;
	ret->useStderr = true;
	ret->printTime = true;
	return ret;
}
LOGGER_logger logger_create_autoln_with_file(LOGGER_loglevel level, bool useAutoNewline, const char* filename, bool useStdout, bool useStderr)
{
	LOGGER_logger ret = calloc(sizeof(struct __logger), sizeof(struct __logger));
	if (ret == NULL)
		return NULL;
	ret->filename = calloc(strlen(filename) + 1, sizeof(char));
	if (!ret->filename)
	{
		ret->ec = LOGGER_error_bad_alloc;
		ret->good = false;
		return ret;
	}
	for (int i = 0; i < strlen(filename); i++)
		ret->filename[i] = filename[i];
#pragma warning(push)
#pragma warning(disable : 4996)
	ret->filehandle = fopen(ret->filename, "w");
#pragma warning(pop)
	if (!ret->filehandle)
	{
		ret->ec = LOGGER_error_cannot_open_file;
		ret->good = false;
		return ret;
	}
	ret->level = level;
	ret->autoLN = useAutoNewline;
	ret->useStdout = useStdout;
	ret->useStderr = useStderr;
	ret->good = true;
	ret->printTime = true;
	return ret;
}
void logger_destroy(LOGGER_plogger logger)
{
	if((*logger)->filehandle) 
		fclose((*logger)->filehandle);
	free((*logger)->filename);
	(*logger)->filename = NULL;
	free(*logger);
	*logger = NULL;
}
LOGGER_logerror logger_get_error(LOGGER_logger logger)
{
	return logger->ec;
}
int logger_internal_count(const char* string, char c)
{
	int i, ret = 0;
	for (i = 0; i < strlen(string); i++)
		if (string[i] == c)
			ret++;
	return ret;
}
int logger_internal_vfprintf(FILE* handle, const char* format, va_list list)
{
	if (handle == NULL)
		return 0;
	int len = vfprintf(handle, format, list);
	fflush(handle);
	return len;
}
int logger_internal_fprintf(FILE* handle, const char* format, ...)
{
	if (handle == NULL)
		return 0;
	va_list list;
	__crt_va_start(list, format);
	int len = vfprintf(handle, format, list);
	__crt_va_end(list);
	fflush(handle);
	return len;
}
int logger_internal_puts(LOGGER_logger logger, const char* buf)
{
	int size1 = 0;
	int size2 = logger_internal_fprintf(logger->filehandle, "%s", buf);
	if (logger->useStdout)
		size1 = fputs(buf, stdout);
	else
		size1 = size2;
	if (!size1 || !size2)
	{
		logger->ec = LOGGER_error_general_file_error;
		logger->good = false;
		return 0;
	}
	return size1 & size2;
}
int logger_internal_eputs(LOGGER_logger logger, const char* buf)
{
	int size1 = 0;
	int size2 = logger_internal_fprintf(logger->filehandle, buf);
	if (logger->useStderr)
		size1 = fputs(buf, stderr);
	else
		size1 = size2;
	if (!size1 || !size2)
	{
		logger->ec = LOGGER_error_general_file_error;
		logger->good = false;
		return 0;
	}
	return size1 & size2;
}
int logger_log(LOGGER_logger logger, const char* format, ...)
{
	if (logger->level != LOGGER_level_log)
	{
		logger->ec = LOGGER_error_level_not_high_enough;
		logger->good = false;
		return 0;
	}
	char time_str[65];
	memset(time_str, 0, 65);
	if (logger->printTime)
	{
		time_t t = time(0);
		struct tm* tm_info = localtime(&t);
		// Ex: 01/01/2022 00:00:00: 
		strftime(time_str, 26, "%d/%m/%Y %H:%M:%S: ", tm_info);
	}
#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LOGGER_colors_green);
	logger_internal_fprintf(logger->filehandle, "%s[INFO] ", time_str);
	fprintf(stdout, "%s[INFO] ", time_str);
#else
	logger_internal_fprintf(logger->filehandle, "\x1b[32m%s[INFO] ", time_str);
	if(logger->useStdout) 
		fprintf(stdout, "\x1b[32m%s[INFO] ", time_str);
#endif
	va_list list;
	__crt_va_start(list, format); 
	int size1 = 0;
	int size2 = logger_internal_vfprintf(logger->filehandle, format, list);
	if (logger->useStdout)
		size1 = vfprintf(stdout, format, list);
	else
		size1 = size2;
	__crt_va_end(list);
	if(logger->filehandle) 
		fflush(logger->filehandle);
#ifndef _WIN32
	if (logger->autoLN)
		logger_internal_puts(logger, "\x1b[0m\n");
	else
		logger_internal_puts(logger, "\x1b[0m");
#else
	if (logger->autoLN)
		logger_internal_puts(logger, "\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LOGGER_colors_white);
#endif
	if (!size1 || !size2)
	{
		logger->ec = LOGGER_error_general_file_error;
		logger->good = false;
		return 0;
	}
	return size1 & size2;
}
int logger_log_warning(LOGGER_logger logger, const char* format, ...)
{
	if (logger->level > LOGGER_level_warning)
	{
		logger->ec = LOGGER_error_level_not_high_enough;
		logger->good = false;
		return 0;
	}
	char time_str[65];
	memset(time_str, 0, 65);
	if (logger->printTime)
	{
		time_t t = time(0);
		struct tm* tm_info = localtime(&t);
		// Ex: 01/01/2022 00:00:00: 
		strftime(time_str, 26, "%d/%m/%Y %H:%M:%S: ", tm_info);
	}
#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LOGGER_colors_yellow);
	logger_internal_fprintf(logger->filehandle, "%s[WARNING] ", time_str);
	fprintf(stderr, "%s[WARNING] ", time_str);
#else
	logger_internal_fprintf(logger->filehandle, "\x1b[93m%s[WARNING] ", time_str);
	if (logger->useStderr)
		fprintf(stderr, "\x1b[93m%s[WARNING] ", time_str);
#endif
	va_list list;
	__crt_va_start(list, format);
	int size1 = 0;
	int size2 = logger_internal_vfprintf(logger->filehandle, format, list);
	if (logger->useStderr)
		size1 = vfprintf(stderr, format, list);
	else
		size1 = size2;
	__crt_va_end(list);
	if (logger->filehandle)
		fflush(logger->filehandle);
#ifndef _WIN32
	if (logger->autoLN)
		logger_internal_eputs(logger, "\x1b[0m\n");
	else
		logger_internal_eputs(logger, "\x1b[0m");
#else
	if (logger->autoLN)
		logger_internal_eputs(logger, "\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LOGGER_colors_white);
#endif
	if (!size1 || !size2)
	{
		logger->ec = LOGGER_error_general_file_error;
		logger->good = false;
		return 0;
	}
	return size1 & size2;
}
int logger_log_error(LOGGER_logger logger, const char* format, ...)
{
	if (logger->level > LOGGER_level_error)
	{
		logger->ec = LOGGER_error_level_not_high_enough;
		logger->good = false;
		return 0;
	}
	char time_str[65];
	memset(time_str, 0, 65);
	if (logger->printTime)
	{
		time_t t = time(0);
		struct tm* tm_info = localtime(&t);
		// Ex: 01/01/2022 00:00:00: 
		strftime(time_str, 26, "%d/%m/%Y %H:%M:%S: ", tm_info);
	}
#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LOGGER_colors_lightred);
	logger_internal_fprintf(logger->filehandle, "%s[ERROR] ", time_str);
	fprintf(stderr, "%s[ERROR] ", time_str);
#else
	logger_internal_fprintf(logger->filehandle, "%s[ERROR] ", time_str);
	if (logger->useStderr)
		fprintf(stderr, "\x1b[31m\x1b[1m%s[ERROR] ", time_str);
#endif
	va_list list;
	__crt_va_start(list, format);
	int size1 = 0;
	int size2 = logger_internal_vfprintf(logger->filehandle, format, list);
	if (logger->useStderr)
		size1 = vfprintf(stderr, format, list);
	else
		size1 = size2;
	__crt_va_end(list);
	if (logger->filehandle)
		fflush(logger->filehandle);
#ifndef _WIN32
	if (logger->autoLN)
		logger_internal_eputs(logger, "\x1b[0m\n");
	else
		logger_internal_eputs(logger, "\x1b[0m");
#else
	if (logger->autoLN)
		logger_internal_eputs(logger, "\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LOGGER_colors_white);
#endif
	if (!size1 || !size2)
	{
		logger->ec = LOGGER_error_general_file_error;
		logger->good = false;
		return 0;
	}
	return size1 & size2;
}
int logger_log_fatal_error(LOGGER_logger logger, const char* format, ...)
{
	if (logger->level > LOGGER_level_fatal)
	{
		logger->ec = LOGGER_error_level_not_high_enough;
		logger->good = false;
		return 0;
	}
	char time_str[65];
	memset(time_str, 0, 65);
	if (logger->printTime)
	{
		time_t t = time(0);
		struct tm* tm_info = localtime(&t);
		// Ex: 01/01/2022 00:00:00: 
		strftime(time_str, 26, "%d/%m/%Y %H:%M:%S: ", tm_info);
	}
#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LOGGER_colors_yellow);
	logger_internal_fprintf(logger->filehandle, "%s[FATAL_ERROR] ", time_str);
	fprintf(stderr, "%s[FATAL_ERROR] ", time_str);
#else
	logger_internal_fprintf(logger->filehandle, "\x1b[31m%s[FATAL_ERROR] ", time_str);
	if (logger->useStderr)
		fprintf(stderr, "\x1b[31m%s[FATAL_ERROR] ", time_str);
#endif
	va_list list;
	__crt_va_start(list, format);
	int size1 = 0;
	int size2 = logger_internal_vfprintf(logger->filehandle, format, list);
	if (logger->useStderr)
		size1 = vfprintf(stderr, format, list);
	else
		size1 = size2;
	__crt_va_end(list);
	if (logger->filehandle)
		fflush(logger->filehandle);
#ifndef _WIN32
	if (logger->autoLN)
		logger_internal_eputs(logger, "\x1b[0m\n");
	else
		logger_internal_eputs(logger, "\x1b[0m");
#else
	if (logger->autoLN)
		logger_internal_eputs(logger, "\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LOGGER_colors_white);
#endif
	if (!size1 || !size2)
	{
		logger->ec = LOGGER_error_general_file_error;
		logger->good = false;
		return 0;
	}
	return size1 & size2;
}