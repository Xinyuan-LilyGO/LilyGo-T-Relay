# Write to new users

- This page provides a quick diagnosis of whether the hardware is normal. The firmware in the example table below provides quick testing of the hardware. Please follow the steps below to test

# 1️⃣ Quickly diagnose hardware

| Product                                      | Firmware Name                                                                                               |
| -------------------------------------------- | ----------------------------------------------------------------------------------------------------------- |
| [LilyGo-T-Relay-4][1]                        | [firmware](./LilyGo-T-Relay-4/LilyGo-T-Relay-4-20210816.bin)                                                |
| [LilyGo-T-Relay-8][2]                        | [firmware](./LilyGo-T-Relay-8/LilyGo-T-Relay-8-20220516.bin)                                                |
| [LilyGo-T-Relay-S3][3]                       | [firmware](./LilyGo-T-Relay-S3/firmware_LilyGo-Relay-UnitTest_20230830.bin)                                 |
| [LilyGo-T-Relay_W5500_Ethernet 4 Channel][4] | [firmware](./LilyGo-T-Relay_W5500_EthernetServer_Relay4_V1.0_firmware/W5500_EthernetServer_Relay4_V1.0.bin) |
| [LilyGo-T-Relay_W5500_Ethernet 8 Channel][4] | [firmware](./LilyGo-T-Relay_W5500_EthernetServer_Relay8_V1.0_firmware/W5500_EthernetServer_Relay8_V1.0.bin) |
| [T-Relay6-S3-W5500_Original_Test] | [firmware](./LilyGo-T-Relay6-S3-W5500_Original_Test/[T-Relay6-S3_V1.0][T-Relay6-S3_W5500_Original_Test]_firmware_V1.0.0.bin) |

[1]: https://www.lilygo.cc/products/t-relay
[2]: https://www.lilygo.cc/products/t-relay-5v-8-channel-relay
[3]: https://www.lilygo.cc/products/t-relay-s3
[4]: https://www.lilygo.cc/products/t-relay?variant=43164455207093


### Use ESP Download Tool

- Download [Flash_download_tool](https://www.espressif.com.cn/sites/default/files/tools/flash_download_tool_3.9.5_0.zip)

![web_flasher](./images/esp_downloader.gif)

* Note that after writing is completed, you need to press RST to reset.


### Use Web Flasher

- [ESP Web Flasher Online](https://espressif.github.io/esptool-js/)

![web_flasher](./images/web_flasher.gif)

* Note that after writing is completed, you need to press RST to reset.

### Use command line


If system asks about install Developer Tools, do it.

```
python3 -m pip install --upgrade pip
python3 -m pip install esptool
```

In order to launch esptool.py, exec directly with this:

```
python3 -m esptool
```

For ESP32-S3 use the following command to write

```
esptool --chip esp32s3  --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m 0x0 firmware.bin

```

# 2️⃣FAQ

- Can't upload any sketch，Please enter the upload mode manually.
   1. Connect the board via the USB cable
   2. Press and hold the **BOOT** button , While still pressing the **BOOT** button (If there is no **BOOT** button, you need to use wires to connect **GND** and **IO0** together.)
   3. Press **RST** button
   4. Release the **RST** button
   5. Release the **BOOT** button (If there is no **BOOT** button, disconnect IO0 from GND.)
   6. Upload sketch
   7. Press the **RST** button to exit download mode


