#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

int main() {
	stdio_init_all();
	while(true) {
		printf("LED On\n");
		cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
		sleep_ms(500);
		printf("LED Off\n");
		cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
		sleep_ms(500);
	}
	return 0;
}
