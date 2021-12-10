# ESP8266-Water-Temperature
Simple Async HTTP mDNS Server for ESP8266 with DS18B20 temperature sensor to measure water temperature.

# Installation
Connect Dallas DS18B20 to your ESP8266 as follows:
1. You need to have a 4.7k Ohm resistor connected between red wire (+) and yellow wire (DATA)
2. Connect yellow wire (DATA) to `D2` pin
3. Connect black wire (GND or -) to `GND` or `G` pin
4. Connect red wire (+) to `3V` pin

Firmware flash:
1. Install PlatformIO
2. Open project
3. Rename `src/network_settings_EXAMPLE.h` to `src/network_settings.h`
3. In `src/network_settings.h` change WiFi SSID and password
4. Compile
5. Upload compiled firmware to your ESP8266

You can also use Arduino IDE to flash. In that case you need to copy whole `src/main.cpp` and `src/network_settings.h` files to Arduino sketch and install proper libraries listed in `platformio.ini` file under `lib_deps` property.

# Usage
### Finding IP Address of ESP8266
You can obtain IP Address using:
- Serial monitor
- mDNS service discovery:   
service type: `_magpie_home._tcp.local.`   
service name: `esp_water_temp._magpie_home._tcp.local.`

You can edit mDNS service in `src/network_settings.h` file.

### Connecting
- `http://<ESP_IP_ADDRESS>/` - simple page showing IP Address, hostname and temperature
- `http://<ESP_IP_ADDRESS>/temp` - raw formatted temperature in celsius