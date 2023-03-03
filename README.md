# logger
 - A logger for WIN64 "C", or LINUX "C".
## API Documentation:
  ```c 
        LOGGER_logger logger_create(LOGGER_loglevel level);
	    LOGGER_logger logger_create_autoln(LOGGER_loglevel level, bool useAutoNewline);
	    LOGGER_logger logger_create_autoln_with_file(LOGGER_loglevel level, bool useAutoNewline, const char* filename, bool useStdout, bool useStderr);
	    void logger_destroy(LOGGER_plogger logger);
	    LOGGER_logerror logger_get_error(LOGGER_logger logger);
	    int logger_log(LOGGER_logger logger, const char* format, ...);
	    int logger_log_warning(LOGGER_logger logger, const char* format, ...);
	    int logger_log_error(LOGGER_logger logger, const char* format, ...);
	    int logger_log_fatal_error(LOGGER_logger logger, const char* format, ...);
      - logger_create:
        Parameters:
          LOGGER_loglevel level - The logger level (LOGGER_level_log accepting all, LOGGER_level_fatal only fatal errors)
        Returns:
           LOGGER_logger: The constructed object (Must call logger_destroy before the object exits the scope or bad stuff will happen).
      - logger_create_autoln:
        Parameters:
          LOGGER_loglevel level - The logger level (LOGGER_level_log accepting all, LOGGER_level_fatal only fatal errors),
          bool useAutoNewLine   - Whether to automatically print a newline at the end of the stream or not.
        Returns:
          LOGGER_logger: The constructed object (Must call logger_destroy before the object exits the scope or bad stuff will happen).
      - logger_create_autoln_with_file:
        Parameters:
          LOGGER_loglevel level - The logger level (LOGGER_level_log accepting all, LOGGER_level_fatal only fatal errors),
          bool useAutoNewLine   - Whether to automatically print a newline at the end of the stream or not,
          const char* filename  - The log file (If constructed with any of the other ones, no file is used),
          bool useStdOut        - Whether to print to stdOut or not.
          bool useStdErr        - Whether to print to stdErr or not.
        Returns:
          LOGGER_logger: The constructed object with a file handle and file name (Must call logger_destroy before the object exits the scope or bad stuff will happen).
          - logger_destroy:
            Parameters:
              LOGGER_plogger logger - A pointer to the constructed object.
            Returns:
              void
          - logger_get_error:
            Parameters:
              LOGGER_logger logger - The object to get the error.
            Returns:
              LOGGER_logerror - The error.
          - logger_log
            Parameters:
              LOGGER_logger logger - The object to print on,
              const char* format   - The printf-style format string.
              ...                  - The extra arguments.
            Returns:
              int - The number of bytes written, or 0 on error.
            Notes:
              In the object, there is a property "printTime". This defaults to true on all constructors, so if you don't want to print the time, set this to false.
              Prints to stdOut.
            - logger_log_warning
            Parameters:
              LOGGER_logger logger - The object to print on,
              const char* format   - The printf-style format string.
              ...                  - The extra arguments.
            Returns:
              int - The number of bytes written, or 0 on error.
            Notes:
              In the object, there is a property "printTime". This defaults to true on all constructors, so if you don't want to print the time, set this to false.
              Prints to stdErr.
             - logger_log_error
            Parameters:
              LOGGER_logger logger - The object to print on,
              const char* format   - The printf-style format string.
              ...                  - The extra arguments.
            Returns:
              int - The number of bytes written, or 0 on error.
            Notes:
              In the object, there is a property "printTime". This defaults to true on all constructors, so if you don't want to print the time, set this to false.
              Prints to stdErr.
              - logger_log_fatal_error
            Parameters:
              LOGGER_logger logger - The object to print on,
              const char* format   - The printf-style format string.
              ...                  - The extra arguments.
            Returns:
              int - The number of bytes written, or 0 on error.
            Notes:
              In the object, there is a property "printTime". This defaults to true on all constructors, so if you don't want to print the time, set this to false.
              Prints to stdErr.
