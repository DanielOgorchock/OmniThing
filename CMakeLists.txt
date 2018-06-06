cmake_minimum_required(VERSION 3.11)
project(OmniThing)
message("Running OmniThing's CMakeLists.txt")

option(BUILD_LINUX "Build for a generic linux target" OFF)
option(BUILD_WINDOWS "Build for a windows target" OFF)
option(BUILD_RPI "Build for a raspberry pi linux target" OFF)
option(BUILD_ARDUINO "Create directory structure for Arduino IDE" OFF)
set(ARDUINO_CONFIG "arduino_config.json" CACHE STRING "Which header file to use for config")

file(GLOB_RECURSE sources_core src/core/*.cpp src/core/*.c)
file(GLOB_RECURSE sources_os_common src/os_common/lib/*.cpp)
file(GLOB_RECURSE sources_rpi src/raspberry_pi/lib/*.cpp)
file(GLOB_RECURSE sources_arduino src/arduino/lib/*.cpp)
file(GLOB_RECURSE sources_embedded_common src/embedded_common/lib/*.cpp)

file(GLOB_RECURSE includes_core include/core/*.h)
file(GLOB_RECURSE includes_os_common include/os_common/lib/*.h)
file(GLOB_RECURSE includes_rpi include/raspberry_pi/lib/*.h)
file(GLOB_RECURSE includes_arduino include/arduino/lib/*.h)
file(GLOB_RECURSE includes_embedded_common include/embedded_common/lib/*.h)

file(GLOB sources_ESP32WebServer src/ESP32WebServer/*)

if(BUILD_LINUX)
    message("Creating Linux Build Configuration")
    add_executable(OmniThing src/os_common/main_os.cpp ${sources_core} ${sources_os_common})
    include_directories(include/core)
    include_directories(include/os_common/lib)
    target_link_libraries(OmniThing pthread)

    add_definitions("-DOMNI_FAKE_F_MACRO")
    add_definitions("-DOMNI_PLAT_LINUX")
    add_definitions("-DOMNI_NOT_ARDUINO")
elseif(BUILD_RPI)
    message("Creating Raspberry Pi Build Configuration")
    add_executable(OmniThing src/os_common/main_os.cpp  ${sources_core} 
                                                        ${sources_os_common} 
                                                        ${sources_rpi} 
                                                        ${sources_embedded_common}
                                                        )
    include_directories(include/core)
    include_directories(include/os_common/lib)
    include_directories(include/raspberry_pi/lib)
    include_directories(include/embedded_common/lib)
    target_link_libraries(OmniThing pigpio pthread rt)

    add_definitions("-DOMNI_FAKE_F_MACRO")
    add_definitions("-DOMNI_PLAT_RPI")
    add_definitions("-DOMNI_NOT_ARDUINO")
elseif(BUILD_WINDOWS)
	message("Creating Windows Build Configuration")
	add_executable(OmniThing src/os_common/main_os.cpp ${sources_core} ${sources_os_common})
	add_definitions(-DFOO)
	include_directories(include/core)
	include_directories(include/os_common/lib)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT OmniThing)
	find_library(ws2_32_LIBRARY_PATH ws2_32)
	target_link_libraries(OmniThing ${ws2_32_LIBRARY_PATH})

    add_definitions("-DOMNI_FAKE_F_MACRO")
    add_definitions("-DOMNI_PLAT_WINDOWS")
    add_definitions("-DOMNI_NOT_ARDUINO")
elseif(BUILD_ARDUINO)
    message("Creating Arduino Build Configuration")
    file(GLOB_RECURSE old_files arduino_build/*)
    file(REMOVE ${old_files})
    file(COPY src/arduino/OmniThing.ino ${sources_core} 
                                        ${sources_arduino} 
                                        ${includes_core} 
                                        ${includes_arduino} 
                                        ${sources_ESP32WebServer} 
                                        ${sources_embedded_common}
                                        ${include_embedded_common}
                                        DESTINATION arduino_build/OmniThing)
    execute_process(COMMAND python jsonToHeader.py ${ARDUINO_CONFIG} arduino_build/OmniThing/ArduinoJsonConfig.h)
endif()


#unset(BUILD_LINUX CACHE)
#unset(BUILD_WINDOWS CACHE)
#unset(BUILD_RPI CACHE)
#unset(BUILD_ARDUINO CACHE)
