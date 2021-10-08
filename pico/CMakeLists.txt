cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(myproject_test C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
add_compile_definitions(PLATFORM_PICO)
include_directories(../pico)

pico_sdk_init()

add_executable(syntheyes
	../syntheyes.cpp
	main.cpp
	Hub08.cpp
	../ColourWheel.cpp
	WS2812.cpp
	PicoMic.cpp
	PicoTiming.cpp
	)

pico_generate_pio_header(syntheyes ${CMAKE_CURRENT_LIST_DIR}/hub75.pio)
pico_generate_pio_header(syntheyes ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

target_link_libraries(syntheyes pico_stdlib hardware_pio hardware_adc)

#pico_enable_stdio_usb(blink 1)
#pico_enable_stdio_uart(blink 1)

pico_add_extra_outputs(syntheyes)

