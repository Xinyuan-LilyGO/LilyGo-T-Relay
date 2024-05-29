<!--
 * @Description: None
 * @version: V1.0.0
 * @Author: None
 * @Date: 2024-05-29 14:38:46
 * @LastEditors: LILYGO_L
 * @LastEditTime: 2024-05-29 16:53:51
 * @License: GPL 3.0
-->



<h1 align = "center">✨ LilyGO T-Relay S3 ✨</h1>

## **[English](README.MD) | 中文**


## 1️⃣ 示例预览

```
examples 
├─DS18x20_Temperature            #外接DS18B20示例     
├─HomeAssistant                  #简易配置就可以接入家庭助理 +OLED SS1306  示例
├─NimBLE_Server                  #使用BLE 控制继电器        
├─RTC_DS3231_External_Module   # 使用 外部接入 DS3231 RTC模块
├─RTC_PCF8563_TimeSynchronization    # 使用板载 PCF8563 RTC 
├─TFT_eSPI                       #使用2.4'寸电阻触摸屏 
├─UnitTestExample                #测试程序     
└─T-Relay6-S3-W5500_Original_Test        #T-Relay6-S3外插W5500出厂测试程序     

```


## 2️⃣ 快速开始

### PlatformIO

1. 安装[VisualStudioCode](https://code.visualstudio.com/) 和 [Python](https://www.python.org/)
2. 在`VisualStudioCode`扩展中搜索`PlatformIO`插件并安装.
3. 安装完成后需要将`VisualStudioCode`重新启动
4. 重新开启`VisualStudioCode`后,选择 `VisualStudioCode` 左上角的 `文件` -> `打开文件夹` ->选择 `LilyGo-T-Relay` 目录
5. 点击 `platformio.ini` 文件,在`platformio` 栏目中取消需要使用的示例行,请确保仅仅一行有效
6. 点击左下角的（✔）符号编译
7. 将板子与电脑USB进行连接
8. 点击（→）上传固件
9. 点击 (插头符号) 监视串行输出s


### ArduinoIDE


1. 安装 [ArduinoIDE](https://www.arduino.cc/en/software)
2. 将 `LilyGo-T-Relay/lib` 目录内的所有文件夹拷贝到`<C:\Users\UserName\Documents\Arduino\libraries>`,如果没有`libraries`目录,请新建,请注意,不是拷贝`lib`目录,而是拷贝lib目录里面的文件夹
3. 打开ArduinoIDE -> Tools 
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
4. 插入USB到PC,点击上传<如果无法顺利上传,请保持按压BOOT按键,然后单击RST,然后再点击上传,上传完成时需要点击RST退出下载模式

### ESPHome

1. 请查看参考[配置文件](./esphome/relay.yaml)
2. 在使用过程中,ESPHome得到的效果并不好,不建议使用,如果需要接入HomeAssistant请使用 <examples/HomeAssistant> 草图，可以通过入网配置期间配置继电器数量，MQTT服务器参数 ,HomeAssistant如果开启了自动发现,那么 `T-Relay`能够被 HomeAssistant 自动发现.



## 3️⃣ 提示:

1. ⚠`T-Relay` 使用 `Octal SPI` Flash ,所以 **GPIO35 ~ GPIO37** 为不可用,请不要在软件中使用这几个IO
2. ⚠`T-Relay` **3v3*** 外部允许最大电流为**500mA**，请不要超过此供电电流
3. ⚠`T-Relay` **5V** Pin 最大允许电流为3000mA，ESP32S3 和其他外部元器件需要将近800mA电流，所以请不要在外部连接大于**2000mA**的元器件
4. ⚠`T-Relay` 继电器引脚触电最大允许电流为10A负载电流，请不要连接超过**2500W**的负载设备
5. ⚠**如果外接大于48V的继电器负载，请先将主板和连接设备进行断电，防止触电.**
6. ⚠`T-Relay` 为`74HC595`串行转并行设计,允许多个`T-Relay`副板进行连接,但是请注意，受限于主板的供电的设计,目前建议 级联设备不超过 3 个继电器副板，级联三个副板，主板电源会发烫这是正常的，请不要用手触摸,防止烫伤,如果长时间工作，建议在电源电感处粘贴散热器辅助主板散热.
7. ⚠ 默认预留DS18B20传感器接口,默认是没有这个传感器的


## 4️⃣ ESP32S3 引脚分布:

| Shift register | DATA | CLOCK | LATCH |
| -------------- | ---- | ----- | ----- |
| GPIO           | 7    | 5     | 6     |

| RTC  | SDA | SCL | IRQ |
| ---- | --- | --- | --- |
| GPIO | 16  | 17  | 18  |

## 5️⃣ 移位寄存器硬件分布:

| Relay Channel | CH1 | CH2 | CH3 | CH4 | CH5 | CH6 | CH7       | CH8     |
| ------------- | --- | --- | --- | --- | --- | --- | --------- | ------- |
| GPIO          | SR1 | SR2 | SR3 | SR4 | SR5 | SR6 | GREEN LED | RED LED |

## 6️⃣ T-Relay6-S3-W5500 引脚:

| T-Relay6-S3-W5500 引脚  | ESP32S3 引脚|
| :------------------: | :------------------:|
| SCLK         | IO9       |
| MISO         | IO11       |
| MOSI         | IO13       |
| CS         | IO3       |
| RST         | IO8       |
| INT         | IO12       |

