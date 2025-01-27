#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"
#include "config.h"

static uint8_t stripes_data[WS2812_PIXELS * 3];
static uint8_t init_data[3*3] = {255, 0, 0, 0, 255, 0, 0, 0, 255};

static void put_pixel(PIO pio, uint sm, uint8_t *data, int n) {
	uint8_t* start = data + n * 3;
	uint32_t pixel = (start[0] << 16) | (start[1] << 8) | start[2];
	pio_sm_put_blocking(pio, sm, pixel << 8u);
}

static void write_ws2812(PIO pio, uint sm) {
	for(int i = 0; i < WS2812_PIXELS; i++) {
		put_pixel(pio, sm, stripes_data, i);
	}
}

int main() {
	stdio_init_all();
	PIO pio;
	uint sm;
	uint offset;

	bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio, &sm, &offset, WS2812_PIN, 1, true);
	hard_assert(success);
	ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);
	
	memcpy(stripes_data, init_data, sizeof(init_data));

	while(true) {
		write_ws2812(pio, sm);
		sleep_ms(1000);
	}
	pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset);
	return 0;
}
