cmake_minimum_required(VERSION 2.8)

add_definitions(-D__LINUX_ALSASEQ__)
add_library(Midi Midi.C RtMidi.C)
target_include_directories (Midi PUBLIC $(CMAKE_CURRENT_SOURCE_DIR))
