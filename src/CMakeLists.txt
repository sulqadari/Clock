add_executable(${ENTRY_POINT}.elf
	main.c
	startup_stm32f103xb.s
	system_stm32f1xx.c
	${hal_path}/clock_src.c
)

target_include_directories(${ENTRY_POINT}.elf
	PUBLIC ${PROJECT_SOURCE_DIR}/inc
	PUBLIC ${PROJECT_SOURCE_DIR}/inc/CMSIS
)

add_custom_command(TARGET ${ENTRY_POINT}.elf
	POST_BUILD
	COMMAND ${OBJCOPY} -O binary ${ENTRY_POINT}.elf ${PROJECT_SOURCE_DIR}/${ENTRY_POINT}.bin
	BYPRODUCTS SeOS.bin
)

add_custom_command(TARGET ${ENTRY_POINT}.elf
	POST_BUILD
	COMMAND ${OBJCOPY} -O ihex ${ENTRY_POINT}.elf ${ENTRY_POINT}.hex
	BYPRODUCTS ${ENTRY_POINT}.hex
)

add_custom_command(TARGET ${ENTRY_POINT}.elf
	POST_BUILD
	COMMAND ${SIZE} ${ENTRY_POINT}.elf
)
