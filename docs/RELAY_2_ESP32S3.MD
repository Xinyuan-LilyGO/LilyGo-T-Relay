<h1 align = "center">✨ LilyGO T-Relay-S3  2-Channel ✨</h1>

## 1️⃣ Examples preview

````

examples 
├── Relay_2_Blynk_Console        ; Blynk example
├── Relay_2_Blynk_WiFi           ; Blynk example
├── Relay_2_Factory              ; Factory test
├── Relay_2_InteractiveWebServer ; WebServer control relay
└── Relay_2_simple               ; Simple test relay

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
- Flash Size -> 4MB(32Mb)
- USB Firmware MSC On Boot -> Disable
- PSRAM -> Disabled
- Partition Scheme -> Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)
- USB Mode -> Hardware CDC and JIAG
- Upload Mode -> UART0/Hardware CDC
- Upload Speed -> 921600

## 3️⃣ Tips

1. The board uses USB as the JTAG upload port. When printing serial port information on USB_CDC_ON_BOOT configuration needs to be turned on. 
If the port cannot be found when uploading the program or the USB has been used for other functions, the port does not appear. 
Please enter the upload mode manually.
   1. Connect the board via the USB cable
   2. Press and hold the BOOT button , While still pressing the BOOT button, press RST
   3. Release the RST
   4. Release the BOOT button
   5. Upload sketch

## 4️⃣ ESP32S3 Pins

| Relay num | ESP32S3 Pins |
| :-------: | :----------: |
|  Relay 1  |     IO35     |
|  Relay 2  |     IO36     |

## QWIIC I2C Pins

| Name  | ESP32S3 Pins |
| :---: | :----------: |
|  SDA  |     IO16     |
|  SCL  |     IO17     |

## QWIIC UART Pins

| Name  | ESP32S3 Pins |
| :---: | :----------: |
|  TXD  |     IO43     |
|  RXD  |     IO44     |
