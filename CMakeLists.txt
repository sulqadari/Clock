cmake_minimum_required(VERSION 3.25)
project(Clock VERSION 1.0)


enable_language(ASM C)
set(CMAKE_C_COMPILER	"arm-none-eabi-gcc")
set(CMAKE_ASM_COMPILER	"arm-none-eabi-gcc")
set(CMAKE_LINKER		"arm-none-eabi-ld")
set(OBJCOPY				"arm-none-eabi-objcopy")
set(OBJDUMP				"arm-none-eabi-objdump")
set(SIZE				"arm-none-eabi-size")

set(ENTRY_POINT "Clock_")

set(hal_path ${PROJECT_SOURCE_DIR}/src/hal)

add_compile_options(
	-Wall -O0 -std=c99 -g
	-mcpu=cortex-m3
	-mthumb
	-fdata-sections -ffunction-sections
)

add_link_options(
	-mcpu=cortex-m3
	-mthumb
	"LINKER:--gc-sections"	# used in conjunction with '-fdata-sections -ffunction-sections'
	"LINKER:-T,${CMAKE_SOURCE_DIR}/src/STM32F103XB_FLASH.ld"
	"LINKER:-Map=feeder.map"
)


add_subdirectory(src)