add_executable(boop
	main.c
)

pico_generate_pio_header(boop ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

pico_enable_stdio_usb(boop 1)
pico_enable_stdio_uart(boop 0)

pico_add_extra_outputs(boop)

target_include_directories(boop PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(boop
	pico_cyw43_arch_lwip_threadsafe_background 
	pico_stdlib
	hardware_pio
	pico_multicore
)

set(WS2812_PIXELS "10" CACHE STRING "Number of WS2812 pixels")
set(WS2812_PIN "2" CACHE STRING "WS2812 data pin")
set(WIFI_SSID "WiFi SSID" CACHE STRING "WiFi SSID")
set(WIFI_PASS "WiFi password" CACHE STRING "WiFi password")
set(WIFI_COUNTRY_CODE "XX" CACHE STRING "WiFi country code (XX = WorldWide)")
set(UDP_PORT "1337" CACHE STRING "UDP port")
configure_file(config.h.in config.h @ONLY)
