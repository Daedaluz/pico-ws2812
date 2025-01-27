# PICO-W ws2812 led controller

Tiny wifi ws2812 controller that simply writes raw udp data to a stripe of ws2812 leds.

## compile

```bash
mkdir build
cd build
ccmake .. \
  -DHOSTNAME=pico-ledz \
  -DWIFI_SSID=<mywifi> \
  -DWIFI_PASS=<wpa2 password> \
  -DWIFI_COUNTRY_CODE=XX \
  -DWIFI_MTU=1500 \
  -DWS2812_PIN=2 \
  -DWS2812_PIXELS=10 \
  -DUDP_PORT=1337
```