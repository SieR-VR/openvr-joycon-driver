cmake_minimum_required(VERSION 3.0.0)
project(joycon VERSION 0.1.0)

file(GLOB SOURCE_FILES *.cc */*.cc *.h */*.h)

add_library(driver_joycon SHARED ${SOURCE_FILES})
target_include_directories(
    driver_joycon PUBLIC
    include/external
    include
)

target_link_libraries(driver_joycon
    ${CMAKE_CURRENT_SOURCE_DIR}/include/external/JSL/x64/JoyShockLibrary.lib
)

add_custom_command(TARGET driver_joycon POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:driver_joycon>"
     "${CMAKE_CURRENT_SOURCE_DIR}/joycon/bin/win64/$<TARGET_FILE_NAME:driver_joycon>")