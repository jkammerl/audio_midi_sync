cmake_minimum_required (VERSION 2.8)
project (AudioMidiClockSync)

set (CMAKE_CXX_STANDARD 11)

#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set(CMAKE_BUILD_TYPE "Debug")

SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/")

add_subdirectory(third_party)

add_executable (AudioMidiClockSync src/main.cpp src/beat_tracker.cpp src/audio_io.cpp)
include_directories("src", ${PortAudio_INCLUDE_DIRS})

target_link_libraries (AudioMidiClockSync LINK_PUBLIC PortAudio kissfft BTrack)
install (TARGETS AudioMidiClockSync DESTINATION bin)

