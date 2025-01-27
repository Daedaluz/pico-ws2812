# PICO-W ws2812 led controller

Tiny wifi ws2812 controller based on the RPI Pico W that simply writes raw udp data to a stripe of ws2812 leds.

The color order is normalized to RGB instead of GRB typically for ws2812.

## Protocol

Just simple udp:

Payload: `FF0000  00FF00 0000FF`

| byte   | 0  | 1 | 2   | color | 
|--------|----|----|----|-------|
| pixel1 | FF | 00 | 00 | Red   |
| pixel2 | 00 | FF | 00 | Green |
| pixel3 | 00 | 00 | FF | Blue  |

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