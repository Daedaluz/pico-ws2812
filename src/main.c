#include "config.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "hardware/pio.h"
#include "lwip/netif.h"
#include "ws2812.pio.h"

static uint8_t stripes_data[WS2812_PIXELS * 3];
static uint8_t init_data[3*3] = {20, 0, 0, 0, 20, 0, 0, 0, 20};

static void put_pixel(PIO pio, uint sm, uint8_t *data, int n) {
	uint8_t* start = data + n * 3;
	uint32_t pixel = (start[1] << 16) | (start[0] << 8) | start[2];
	pio_sm_put_blocking(pio, sm, pixel << 8u);
}

static void write_ws2812(PIO pio, uint sm) {
	for(int i = 0; i < WS2812_PIXELS; i++) {
		put_pixel(pio, sm, stripes_data, i);
	}
}

static void led_main() {
	PIO pio;
	uint sm;
	uint offset;
	bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio, &sm, &offset, WS2812_PIN, 1, true);
	hard_assert(success);
	ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);
	memcpy(stripes_data, init_data, sizeof(init_data));

	while(true) {
		write_ws2812(pio, sm);
		sleep_ms(1);
	}

	pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset);
}

static void recv(void* _unused, struct udp_pcb* pcb, struct pbuf* pbuf, const ip_addr_t* src, uint16_t port) {
	//printf("recv from %s:%d  %d %d %p\n", ipaddr_ntoa(src), port, pbuf->len, pbuf->tot_len, pbuf->next);
	int to_copy = pbuf->len < sizeof(stripes_data) ? pbuf->tot_len : sizeof(stripes_data);
	int copied = 0;
	int chunks = 0;
	struct pbuf* p = pbuf;
	while(pbuf != NULL && copied < to_copy) {
		int cap = pbuf->len < to_copy - copied ? pbuf->len : to_copy - copied;
		memcpy(stripes_data + copied, pbuf->payload, cap);
		copied += cap;
		pbuf = pbuf->next;
		chunks++;
	}
	pbuf_free(p);
	//printf("copied %d (%d chunks)\n", copied, chunks);
}

int main() {
	stdio_init_all();
	multicore_launch_core1(led_main);
	sleep_ms(1000);

	while(cyw43_arch_init() != 0) {
		printf("WiFi initialization failed\n");
		sleep_ms(1000);
	};

	cyw43_arch_enable_sta_mode();
	struct netif* nif = netif_list;
	netif_set_hostname(nif, HOSTNAME);
	nif->mtu = WIFI_MTU;
	printf("wifi initialized;%s mtu: %d hostname: %s\n", nif->name, nif->mtu, HOSTNAME);
	//netif_set_mtu(nif, WIFI_MTU);
	while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000) != 0) {
		printf("WiFi connection failed \"%s\"\n", WIFI_SSID);
		sleep_ms(1000);
	};

	struct udp_pcb* udp = udp_new();
	udp_bind(udp, IP_ANY_TYPE, UDP_PORT);
	udp_recv(udp, recv, NULL);
	while(true) {
		// Blink with wifi-led
		cyw43_arch_gpio_put(0, true); 
		sleep_ms(1000);
		cyw43_arch_gpio_put(0, false);
		sleep_ms(1000);
	}

	return 0;
}
