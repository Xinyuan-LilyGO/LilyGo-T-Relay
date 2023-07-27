<h1 align = "center">✨ LilyGO T-Relay S3  ✨</h1>

## **English | [中文](README_CN.MD)**

## 1️⃣ Examples preview

````

examples 
├─DS18x20_Temperature          # External DS18B20 Example，The default is that there is no such sensor, which requires an external connection
├─HomeAssistant                # Example of easy configuration for accessing Home Assistant+OLED SS1306
├─NimBLE_Server                # uses BLE to control the relay
├─RTC_DS3231                   # using DS3231 RTC module
├─TFT_ESPI                     # using a 2.4 'inch resistive touch screen
└─UnitTestExample              # Test program

````

## 2️⃣ Quick Start

### PlatformIO

1. Install [VisualStudioCode](https://code.visualstudio.com/) and [Python](https://www.python.org/)
2. Search for the `PlatformIO` plugin in the `VisualStudioCode` extension and install it.
3. After the installation is complete, you need to restart `VisualStudioCode`
4. After restarting `VisualStudioCode`, select `File` in the upper left corner of `VisualStudioCode` -> `Open Folder` -> select the `LilyGo-T-Relay` directory
5. Click on the `platformio.ini` file, and in the `platformio` column, cancel the sample line that needs to be used, please make sure that only one line is valid
6. Click the (✔) symbol in the lower left corner to compile
7. Connect the board to the computer USB
8. Click (→) to upload firmware
9. Click (plug symbol) to monitor serial output


### ArduinoIDE

1. Install [ArduinoIDE](https://www.arduino.cc/en/software)
2. Copy all the folders in the `LilyGo-T-Relay/lib` directory to `<C:\Users\UserName\Documents\Arduino\libraries>`. If there is no `libraries` directory, please create a new one. Please note that you are not copying the `lib` directory, but copying the folders in the lib directory
3. Open ArduinoIDE -> Tools
- Board -> ESP32S3 Dev Module
- USB CDC On Boot -> Enable
- CPU Frequency -> 240MHz
- USB DFU On Boot -> Disable
- Flash Mode -> QIO 80MHz
- Flash Size -> 16MB(128Mb)
- USB Firmware MSC On Boot -> Disable
- PSRAM -> OPI PSRAM
- Partition Scheme -> 16M Flash(3MB APP/9.9MB FATFS)
- USB Mode -> Hardware CDC and JIAG
- Upload Mode -> UART0/Hardware CDC
- Upload Speed -> 921600





## 3️⃣ Tips:

1. ⚠`T-Relay` uses `Octal SPI` Flash, so **GPIO35 ~ GPIO37** are not available, please do not use these IOs in the software
2. ⚠`T-Relay` **3v3*** The maximum external current allowed is **500mA**, please do not exceed this supply current
3. ⚠`T-Relay` **5V** Pin The maximum allowable current is 3000mA, ESP32S3 and other external components require nearly 800mA current, so please do not connect external components greater than **2000mA**
4. ⚠`T-Relay` The maximum allowable current for electric shock on relay pins is 10A load current, please do not connect load devices exceeding **2500W**
5. ⚠**If an external relay load greater than 48V is connected, please power off the main board and connected equipment first to prevent electric shock.**
6. ⚠`T-Relay` is a serial-to-parallel design for `74HC595`, allowing multiple `T-Relay` sub-boards to be connected, but please note that due to the design of the power supply of the main board, it is currently recommended that cascaded devices not More than 3 relay sub-boards, cascading three sub-boards, the power supply of the main board will be hot, which is normal, please do not touch it with your hands to prevent burns. If you work for a long time, it is recommended to paste a heat sink on the inductor of the power supply to assist the main board to dissipate heat.
7. ⚠ The DS18B20 sensor interface is reserved by default, and there is no such sensor by default
8. External reserved expansion screen interface [Support TFT Module Link](http://www.lcdwiki.com/zh/2.8inch_SPI_Module_ILI9341_SKU:MSP2807)
9. The board uses USB as the JTAG upload port. When printing serial port information on USB_CDC_ON_BOOT configuration needs to be turned on. 
If the port cannot be found when uploading the program or the USB has been used for other functions, the port does not appear. 
Please enter the upload mode manually. 
   1. Connect the board via the USB cable
   2. Press and hold the BOOT button , While still pressing the BOOT button, press RST
   3. Release the RST
   4. Release the BOOT button
   5. Upload sketch

## 4️⃣ ESP32S3 Pins:

| Shift register | DATA | CLOCK | LATCH |
| -------------- | ---- | ----- | ----- |
| GPIO           | 41   | 4     | 5     |

| RTC  | SDA | SCL | IRQ |
| ---- | --- | --- | --- |
| GPIO | 16  | 17  | 15  |

## 5️⃣ Shift register hardware distribution:

| Relay Channel | CH1 | CH2 | CH3 | CH4 | CH5 | CH6 | CH7       | CH8     |
| ------------- | --- | --- | --- | --- | --- | --- | --------- | ------- |
| GPIO          | SR1 | SR2 | SR3 | SR4 | SR5 | SR6 | GREEN LED | RED LED |