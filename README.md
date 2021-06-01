# WemosEM


:warning::warning::warning: **Sorry, I have no time to work on this project. I am involved in new projects** :warning::warning::warning:



<img src="images/wemos-sensor-stc013.jpg" style="max-width: 100%;height: auto;" align="center" />

This is a fork of [@nephiel](https://github.com/Nephiel/MQTT-Power-Sensor) and [@Mottramlabs](https://github.com/Mottramlabs/MQTT-Power-Sensor) projects

Thanks to Dawies [www.domology.es](https://domology.es/medidor-de-consumo-no-invasivo-con-sct013-wemos-d1/) for his contributions and his tutorial

The sensor uses a Wemos D1 Mini and PRO and a Non-invasive Split Core Current Transformer type 100A SCT-013-000, available on eBay.
You can use Current Transformer type calibrated SCT-013-030 (30A), SCT-013-050 (50A) and others. Recomended Clamp 30A or higger.

The sensor samples the current value every 10 seconds and publishes it to a topic via an MQTT broker. you can change this value up to 5 seconds
If the connection fails, it attempts to reconnect to the WiFi and/or the MQTT Broker as needed.

Original Gerber files for the PCB layout and the source files for Kicad are included, although there may be issues with the custom libraries used.

**Software features:**

- Easy setup.
- Responsive design.
- Home Asisstant Discovery.
- All setup in web browser: Wifi, MQTT, Calibrate, timezone, update and system.
- Update Voltaje by MQTT topic.
- NTP to setup timezone.
- Blynk Integration
- ThingSpeak integration
- OTA updates
- Optional reset kWh counter one day of the month
- Reset kWh in MQTT Topic

#### Components: 
- Wemos D1 Mini / D1 Mini Pro
- [ESP8266 Mains Current Sensor](https://www.ebay.es/itm/ESP8266-Mains-Current-Sensor-Wemos-Current-transformer-SCT013-100A-50mA/133077015640)
- Current transformer SCT013 (Ebay or Aliexpress)
- Optional case: https://www.thingiverse.com/thing:3544702

**WARNING:** If you use calibrated clamp, **REMOVE resistance R1** from mains current sensor.

## Firmware installation and first setup

#### Flashing: 

<img src="images/installation01.jpg" alt="installation01" align="right" width="200"/>

You can flash using software [nodemcu-pyflasher](https://github.com/marcelstoer/nodemcu-pyflasher/releases)

1. Download latest source.
2. Rename secrets.h.example to secrets.h and update to taste
3. In variables.h setting Debugging = true will print variable values to serial (remember to set it back to false to avoid credentail leaks)
4. Select Serial port (COM# port) where your serial-to-USB or NodeMCU/D1 mini is connected. Leave on auto-select if not sure.
5. Browse to the binary you downloaded from WemosEM releases.
6. Set Baud rate to 115200 and **Flash mode to DOUT**. Erase flash to **yes** if it is the first time flashing WemosEM on the device or you're experiencing issues with existing flash. If you're upgrading WemosEM set to no.
7. Click Flash NodeMCU and watch the progress in console.

If the flash was successful the console will display:
  Firmware successfully flashed. unplug/replug or reset device to switch back to normal boot mode

Unplug your adapter or device and plug it back in or connect to another power source. Your device is now ready for Initial configuration.

You can see in Tasmota project other options to flash: https://github.com/arendst/Sonoff-Tasmota/wiki/Flashing

#### Initial configuration

WemosEM provides a wireless access point for easy Wi-Fi configuration.

<img src="images/installation02.jpg" alt="installation02" align="right" width="200"/>
<img src="images/installation03.jpg" alt="installation03" align="right" width="200"/>
<img src="images/installation04.jpg" alt="installation04" align="right" width="200"/>

Connect your device to a power source and grab your smartphone (or tablet or laptop or any other web and Wi-Fi capable device). Search for a Wi-Fi AP named **wemosEM-xxxxxx** (where x is a last part of MAC) and connect to it with password **infinito&masalla**. In this example the Wi-Fi AP is named **wemosEM-AB128A**. When it connects to the network, you may get a warning that there is no Internet connection and be prompted to connect to a different network. Do not allow the mobile device to select a different network.

After you have connected to the WemosEM Wi-Fi AP, open http://192.168.4.1 in a web browser on the smartphone (or whatever device you used). Some devices might prompt you to sign in to Wi-Fi network which should also open the above address.

At this page you can have WemosEM scan for available Wi-Fi networks. Select and setup you wifi network. Click save and you wemos will connect to your wifi.

<img src="images/installation05.jpg" alt="installation05" align="right" width="200"/>
Finally, connect with IP or http://wemosem-xxxxxx.local/ and enter default user and passwor:

* **Default user: admin**
* **Default password: admin**

Password is updatable in configuration system tab.

## Ical Value

you can view the ICAL value in http://lechacal.com/wiki/index.php/Current_Transformer_Conditioner

CT | Output Type | Rburden | Rgain (for 5V) | ICAL (for 5V) | Rgain (for 3.3V) | ICAL (for 3.3V)
-- | -- | -- | -- | -- | -- | --
SCT-013-005 | Voltage | None | 130 kΩ | 2.83 | None* | 5.0
SCT-013-010 | Voltage | None | 130 kΩ | 5.66 | None* | 10.0
SCT-013-015 | Voltage | None | 130 kΩ | 8.49 | None* | 15.0
SCT-013-020 | Voltage | None | 130 kΩ | 11.31 | None* | 20.0
SCT-013-025 | Voltage | None | 130 kΩ | 14.14 | None* | 25.0
SCT-013-030 | Voltage | None | 130 kΩ | 16.96 | None* | 30.0
SCT-013-050 | Voltage | None | 130 kΩ | 28.28 | None* | 50.0
SCT-013-060 | Voltage | None | 130 kΩ | 33.94 | None* | 60.0
  |   |   |   |  
SCT-006 | Current | 10 Ω | 17 kΩ | 11.62 | 28 kΩ | 17.5
SCT-013-000 | Current | 10 Ω | 40 kΩ | 56.57 | 75 kΩ | 85.71
SCT-019 | Current | 10 Ω | 24 kΩ | 116.13 | 40 kΩ | 171.43


## Setup in Home Assistant (without MQTT autodiscovery)
    
    sensor:
    - platform: mqtt
      state_topic: "wemos/wemosEM-XXXXXX/power"
      name: WemosEM-Amperaje
      icon: mdi:current-ac
      unit_of_measurement: "A"
      value_template: "{{ value_json.current }}"
    - platform: mqtt
      state_topic: "wemos/wemosEM-XXXXXX/power"
      name: WemosEM-Consumo Actual
      icon: mdi:power-plug
      unit_of_measurement: "W"
      value_template: "{{ value_json.watts }}"
    - platform: mqtt
      state_topic: "wemos/wemosEM-XXXXXX/power"
      name: WemosEM-kWh
      icon: mdi:power-plug
      unit_of_measurement: "kWh"
      value_template: "{{ value_json.kWh }}"nt Cell  | Content Cell  |

## Update Voltaje with Home Assistant

if you have a Shelly EM, add a automation in Home assistant:
      
      - alias: Update voltage WemosEM
        initial_state: 'on'
        trigger:
        - platform: mqtt
          topic: shellies/shellyem-B9E2E9/emeter/0/voltage
    
        action:
        - service: mqtt.publish
          data_template:
            topic: "wemos-cmd/wemosEM-XXXXXX/voltage"
            payload: '{{ trigger.payload }}'

if you have a device with Tasmota, add a automation in Home assistant:

      - alias: Update voltage Tasmota-WemosEM
        initial_state: 'on'
        trigger:
        - platform: mqtt
          topic: tele/sonoff01/SENSOR

        action:
        - service: mqtt.publish
          data_template:
            topic: "wemos-cmd/wemosEM-XXXXXX/voltage"
            payload: "{{ trigger.payload_json['ENERGY'].Voltage }}"

## Blynk Integration

Go to Blynk app on your phone and layout the blynk project with virtual pin number according:
- Virtual PIN V0: Voltage (V)
- Virtual PIN V1: Current (A)
- Virtual PIN V2: Live watts (w)
- Virtual PIN V3: Energy (kWh)
- Virtual PIN V4: Energy before last reset (kWh)

Setup in web interface token auth, optional you can setup server and port server.

## Thingspeak Integration
Setup in web interface token auth (Write API Key) and Channel number. 
- Field 1: Voltage (V)
- Field 2: Current (A)
- Field 3: Live watts (w)
- Field 4: Energy (kWh)
- Field 5: Energy before last reset (kWh)

## MQTT Topics
Topic  | Description
------------- | -------------
wemos-cmd/wemosEM-XXXXXX/voltage | Set/update voltage (from sonoff, shelly or other devices)
wemos-cmd/wemosEM-XXXXXX/status | Get Status of wemosEM
wemos-cmd/wemosEM-XXXXXX/resetkWh | reset kWh and store and beforeResetkWh variable

## OTA Updates
OTA updates from platformio windows with espota:
~/.platformio/packages/tool-espotapy/espota.py --progress --ip 192.168.4.1 --auth "infinito&masalla" --file wemosEM-1.3.1_core-2.4.bin

## Developers
This project is developed with **platformio**. WemosEM not compile with core 2.3 because blynk library require core 2.4.0 or higher.
- Recomended compile this project with Arduino Core 2.4.2.

**Commands:**
- Compile: pio run
- Compile and upload Wemos D1: pio run -t upload

Other options:
- Compile with core 2.5: pio run -t upload -e d1_mini_2_5
- Compile for Wemos D1 Mini PRO (**only core 2.4**): pio run -t upload -e d1_mini_pro_2_4

## Tests

## Screenshots

<img src="images/screenshot01.jpg" style="max-width: 100%;height: auto;" align="center" />
<img src="images/screenshot02.jpg" style="max-width: 100%;height: auto;" align="center" />
<img src="images/screenshot03.jpg" style="max-width: 100%;height: auto;" align="center" />
<img src="images/screenshot04.jpg" style="max-width: 100%;height: auto;" align="center" />
<img src="images/screenshot05.jpg" style="max-width: 100%;height: auto;" align="center" />

