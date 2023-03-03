# CMakeList.txt : CMake project for logger_crossplatform, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (tests "test.cpp" "logger/logger.h")
target_link_libraries(tests logger)

if(WIN32)
	message("OS: WIN32!")
	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		message("Target: WIN32 DEBUG")
		target_compile_options(tests PUBLIC /ZI)
	else()
		message("Target: WIN32 RELEASE")
		target_compile_options(tests PUBLIC /Ot)
	endif()
else()
	message("OS: LINUX")
	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		message("Target: LINUX DEBUG")
		target_compile_options(tests PUBLIC -D_DEBUG PUBLIC -Og)
		target_compile_options(tests PUBLIC -O0)
	else()
		message("Target: LINUX RELEASE")
		target_compile_options(tests PUBLIC -O3)
		add_custom_command(TARGET tests POST_BUILD
		COMMAND strip --strip-debug tests
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Stripping Debug Symbols...")
	endif()
endif()