cmake_minimum_required(VERSION 3.0)
project(OmniThing)

file(GLOB_RECURSE source_files src/core/*.cpp)

add_executable(test ${source_files})

include_directories(include/core)
