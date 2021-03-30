cmake_minimum_required(VERSION 3.20.0)

project(Popeye)

add_subdirectory(Popeye)

add_subdirectory(Popeye/vendor/assimp)

add_subdirectory(Eyeball)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT eyeball)