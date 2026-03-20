CMAKE_MINIMUM_REQUIRED(VERSION 3.28)

include(cmake/arm-none-eabi-gcc.cmake)

PROJECT(TERMINAL_CMSIS_SSD1306 C ASM)

if(CMAKE_SYSTEM_NAME STREQUAL "Generic")
    message("Start building target for embedded hardware.")
endif()


set(TARGET_NAME STM32F103C8T6)
set(OPENOCD_TARGET stm32f1x)
set(SVD_FILE STM32F103xx)
    message(STATUS "Target name was defined \"${TARGET_NAME}\"")
set(LINKER_NAME STM32F103XB_FLASH.ld)
set(MCU_FAMILY cortex-m3)
set(OPTIONS_COMMON -mcpu=${MCU_FAMILY} -mthumb)
set(OPTIONS_OPTIMIZATION -Og -g3)

if(CMAKE_BUILD_TYPE MATCHES Debug)
     set(OPTIONS_OPTIMIZATION -O0 -g3)
endif()
if(CMAKE_BUILD_TYPE MATCHES Release)
     set(OPTIONS_OPTIMIZATION -Os -g0)
endif()

set(OPTIONS_C
    -std=gnu11
    ${OPTIONS_COMMON}
    ${OPTIONS_OPTIMIZATION}
    -ffunction-sections
    -fdata-sections
)
set(OPTIONS_ASM
    ${OPTIONS_COMMON}
    -x
    assembler-with-cpp
)
set(OPTIONS_LINKER ${OPTIONS_COMMON} -T ${CMAKE_SOURCE_DIR}/${LINKER_NAME} -Wl,-Map=${PROJECT_NAME}.map -Wl,--gc-sections)

include(cmake/lib-check.cmake)

message("C compiler options: ${OPTIONS_C}")
message("ASM compiler options: ${OPTIONS_ASM}")
message("Linker options: ${OPTIONS_LINKER}")

require_directory("${CMAKE_SOURCE_DIR}/cmsis"
                  "CMSIS library doesn't set or don't exist.")

add_compile_options("$<$<COMPILE_LANGUAGE:C>:${OPTIONS_C}>")
add_compile_options("$<$<COMPILE_LANGUAGE:ASM>:${OPTIONS_ASM}>")


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



file(GLOB SOURCE_FILES
    src/*.c
    ${CMAKE_SOURCE_DIR}/startup_stm32f103xb.s
)

add_executable(${TARGET_NAME} ${SOURCE_FILES})

target_link_options(${TARGET_NAME} PRIVATE ${OPTIONS_LINKER})
target_link_options(${TARGET_NAME} PRIVATE "LINKER:--no-warn-rwx-segments")


target_include_directories(${TARGET_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/cmsis/Include # Core
    ${CMAKE_SOURCE_DIR}/cmsis/Device/STM32F103/Include  # Target-specific
    ${CMAKE_SOURCE_DIR}/src/headers # Project headers
)

target_compile_definitions(${TARGET_NAME} PRIVATE -DSTM32F103xB)
target_compile_definitions(${TARGET_NAME} PRIVATE -DSTM32F10X_MD)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND arm-none-eabi-size ${TARGET_NAME}.elf
)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary ${TARGET_NAME}.elf ${TARGET_NAME}.bin
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/.vscode/templates/tasks.json.in
    ${CMAKE_CURRENT_SOURCE_DIR}/.vscode/tasks.json
    @ONLY
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/.vscode/templates/launch.json.in
    ${CMAKE_CURRENT_SOURCE_DIR}/.vscode/launch.json
    @ONLY
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/.vscode/templates/settings.json.in
    ${CMAKE_CURRENT_SOURCE_DIR}/.vscode/settings.json
    @ONLY
)
