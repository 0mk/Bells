cmake_minimum_required(VERSION 2.8)

add_library (Socket ClientSocket.C ServerSocket.C Socket.C)
target_include_directories (Socket PUBLIC $(CMAKE_CURRENT_SOURCE_DIR))
