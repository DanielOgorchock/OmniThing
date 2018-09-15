cmake_minimum_required(VERSION 3.7)
project(OmniThing)
message("Running OmniThing's CMakeLists.txt")
message("NOTE: cross compile using -DCMAKE_TOOLCHAIN_FILE= -DRPI_TOOLS=")

set(ARDUINO_CONFIG "arduino_config.json" CACHE STRING "Which json file to use for config")
set(BUILD_TARGET "not_set" CACHE STRING "Which build target (linux, windows, rpi, arduino, website)")
set(ARDUINO_BUILD_LOCATION "arduino_build/OmniThing" CACHE STRING "Where to put arduino sketch output")
set(INSTALL_ROOT "/" CACHE STRING "Where where to treat as root of install")
set(WEB_CONFIG "webpage/config_selfhost.json" CACHE STRING "Which webpage json config to install")
set(RPI_TOOLS "~/programming/git/rpi-tools" CACHE STRING "location of the rapberrypi-tools git repo")

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
file(GLOB sources_ESP32Servo src/ESP32Servo/*)

function(createLinuxInstallTarget)
    install(TARGETS OmniThing
            RUNTIME DESTINATION ${INSTALL_ROOT}/opt/omnithing
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ WORLD_READ
            )
    createWebsiteInstallTarget()
endfunction()

function(createWebsiteInstallTarget)
    install(CODE "
                message(\"Installing with root as ${INSTALL_ROOT}\")
                if(EXISTS ${INSTALL_ROOT}/opt/omnithing/config.json)
                    message(\"config.json exists... installing new blank one as config_new.json\")
                    file(INSTALL webpage/files/blank_config.json DESTINATION ${INSTALL_ROOT}/opt/omnithing
                            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ WORLD_READ
                            RENAME config_new.json
                            )
                else()
                    file(INSTALL webpage/files/blank_config.json DESTINATION ${INSTALL_ROOT}/opt/omnithing
                            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ WORLD_READ
                            RENAME config.json
                            )
                endif()
            ")
    install(FILES install/omnithing.sh DESTINATION ${INSTALL_ROOT}/opt/omnithing
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ WORLD_READ
            )
    install(FILES install/omnithing-webpage.sh DESTINATION ${INSTALL_ROOT}/opt/omnithing
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ WORLD_READ
            )
    install(FILES install/updateConfig.sh DESTINATION ${INSTALL_ROOT}/opt/omnithing
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ WORLD_READ
            )
    install(FILES install/omnithing.service DESTINATION ${INSTALL_ROOT}/etc/systemd/system/
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ WORLD_READ
            )
    install(FILES install/omnithing-webserver.service DESTINATION ${INSTALL_ROOT}/etc/systemd/system/
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ WORLD_READ
            )
    install(FILES ${WEB_CONFIG} DESTINATION ${INSTALL_ROOT}/opt/omnithing
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ WORLD_READ
            RENAME webpage_config.json
            )
    install(DIRECTORY webpage DESTINATION ${INSTALL_ROOT}/opt/omnithing
            FILES_MATCHING PATTERN "*"
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE
            )
    install(DIRECTORY config DESTINATION ${INSTALL_ROOT}/opt/omnithing
            FILES_MATCHING PATTERN "*"
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE
            )
endfunction()

if(BUILD_TARGET STREQUAL "website")
    message("Creating Website Build Configuration")
    createWebsiteInstallTarget()
    install(FILES CMakeLists.txt jsonToHeader.py DESTINATION ${INSTALL_ROOT}/opt/omnithing
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE
            )
    install(DIRECTORY src DESTINATION ${INSTALL_ROOT}/opt/omnithing
            FILES_MATCHING PATTERN "*"
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE
            )
    install(DIRECTORY include DESTINATION ${INSTALL_ROOT}/opt/omnithing
            FILES_MATCHING PATTERN "*"
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE
            )
elseif(BUILD_TARGET STREQUAL "linux")
    message("Creating Linux Build Configuration")
    add_executable(OmniThing src/os_common/main_os.cpp ${sources_core} ${sources_os_common})
    include_directories(include/core)
    include_directories(include/os_common/lib)
    target_link_libraries(OmniThing pthread)

    add_definitions("-DOMNI_FAKE_F_MACRO")
    add_definitions("-DOMNI_PLAT_LINUX")
    add_definitions("-DOMNI_NOT_ARDUINO")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
    createLinuxInstallTarget()
elseif(BUILD_TARGET STREQUAL "rpi")
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
    add_definitions("-DOMNI_PLAT_LINUX")
    add_definitions("-DOMNI_PLAT_RPI")
    add_definitions("-DOMNI_NOT_ARDUINO")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
    createLinuxInstallTarget()
elseif(BUILD_TARGET STREQUAL "windows")
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
elseif(BUILD_TARGET STREQUAL "arduino")
    message("Creating Arduino Build Configuration")
    file(GLOB_RECURSE old_files arduino_build/*)
    if(NOT "${old_files}" STREQUAL "")
        file(REMOVE ${old_files})
    endif()
    file(COPY src/arduino/OmniThing.ino ${sources_core} 
                                        ${sources_arduino} 
                                        ${includes_core} 
                                        ${includes_arduino} 
                                        ${sources_ESP32WebServer} 
                                        ${sources_ESP32Servo} 
                                        ${sources_embedded_common}
                                        ${includes_embedded_common}
                                        DESTINATION ${ARDUINO_BUILD_LOCATION})
    
    message("Writing header file from " ${ARDUINO_CONFIG}  " to " ${ARDUINO_BUILD_LOCATION} "/ArduinoJsonConfig.h")
    execute_process(COMMAND python jsonToHeader.py ${ARDUINO_CONFIG} ${ARDUINO_BUILD_LOCATION}/ArduinoJsonConfig.h)
else()
    message("Invalid BUILD_TARGET specify with -DBUILD_TARGET=[linux, rpi, windows, or arduino]")
endif()

