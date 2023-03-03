# CMakeList.txt : CMake project for logger_crossplatform, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (logger "logger.c" "logger/logger.h" "cross_platform_def.h")

if(WIN32)
	message("OS: WIN32!")
	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		message("Target: WIN32 DEBUG")
		target_compile_options(logger PUBLIC /ZI)
	else()
		message("Target: WIN32 RELEASE")
		target_compile_options(logger PUBLIC /Ot)
	endif()
else()
	message("OS: LINUX")
	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		message("Target: LINUX DEBUG")
		target_compile_options(logger PUBLIC -D_DEBUG PUBLIC -Og)
		target_compile_options(logger PUBLIC -O0)
	else()
		message("Target: LINUX RELEASE")
		target_compile_options(logger PUBLIC -O3)
		# add_custom_command(TARGET logger POST_BUILD
		# COMMAND strip --strip-debug liblogger.a
		# WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		# COMMENT "Stripping Debug Symbols...")
	endif()
endif()