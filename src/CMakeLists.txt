add_executable(boop
	main.c
)

pico_enable_stdio_usb(boop 1)
pico_enable_stdio_uart(boop 0)

pico_add_extra_outputs(boop)

target_include_directories(boop PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(boop pico_cyw43_arch_lwip_threadsafe_background pico_stdlib)

