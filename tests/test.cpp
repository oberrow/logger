#include <stdio.h>
#include <unistd.h>
#include "logger/logger.h"

#ifndef _WIN32
#define _sleep sleep
#endif

int main()
{
	LOGGER_logger l = logger_create(LOGGER_level_log);
	logger_log			  (l, "       Level: %d, %s"  , LOGGER_level_log    , "EEE");
	_sleep(1);
	logger_log_warning	  (l, "    Level: %d, %s"  , LOGGER_level_warning, "EEE");
	_sleep(1);
	logger_log_error	  (l, "      Level: %d, %s"  , LOGGER_level_error  , "EEE");
	_sleep(1);
	logger_log_fatal_error(l, "Level: %d, %s"  , LOGGER_level_fatal  , "EEE");
	l->printTime = false;
	logger_log(l, "       Level: %d, %s", LOGGER_level_log, "EEE");
	_sleep(1);
	logger_log_warning(l, "    Level: %d, %s", LOGGER_level_warning, "EEE");
	_sleep(1);
	logger_log_error(l, "      Level: %d, %s", LOGGER_level_error, "EEE");
	_sleep(1);
	logger_log_fatal_error(l, "Level: %d, %s", LOGGER_level_fatal, "EEE");
	logger_destroy(&l);
	return 0;
}