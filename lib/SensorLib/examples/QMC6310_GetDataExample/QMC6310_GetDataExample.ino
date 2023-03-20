/**
 *
 * @license MIT License
 *
 * Copyright (c) 2022 lewis he
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file      QMC6310_GetDataExample.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @date      2022-10-16
 *
 */
#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>
#include "SensorQMC6310.hpp"

#define I2C1_SDA                    17
#define I2C1_SCL                    18

SensorQMC6310 qmc;

void setup()
{
    Serial.begin(115200);
    while (!Serial);

#ifdef LILYGO_TBEAM_SUPREME_V3_0
    extern  bool setupPower();
    setupPower();
#endif

    if (!qmc.begin(Wire, QMC6310_SLAVE_ADDRESS, I2C1_SDA, I2C1_SCL)) {
        Serial.println("Failed to find QMC6310 - check your wiring!");
        while (1) {
            delay(1000);
        }
    }

    /* Get Magnetometer chip id*/
    Serial.print("Device ID:");
    Serial.println(qmc.getChipID(), HEX);

    /* Config Magnetometer */
    qmc.configMagnetometer(
        /*
        * Run Mode
        * MODE_SUSPEND
        * MODE_NORMAL
        * MODE_SINGLE
        * MODE_CONTINUOUS
        * * */
        SensorQMC6310::MODE_CONTINUOUS,
        /*
        * Full Range
        * RANGE_30G
        * RANGE_12G
        * RANGE_8G
        * RANGE_2G
        * * */
        SensorQMC6310::RANGE_8G,
        /*
        * Output data rate
        * DATARATE_10HZ
        * DATARATE_50HZ
        * DATARATE_100HZ
        * DATARATE_200HZ
        * * */
        SensorQMC6310::DATARATE_200HZ,
        /*
        * Over sample Ratio1
        * OSR_8
        * OSR_4
        * OSR_2
        * OSR_1
        * * * */
        SensorQMC6310::OSR_1,

        /*
        * Down sample Ratio1
        * DSR_8
        * DSR_4
        * DSR_2
        * DSR_1
        * * */
        SensorQMC6310::DSR_1);

    Serial.println("Read data now...");
}

void loop()
{

    //Wiat data ready
    if (qmc.isDataReady()) {

        qmc.readData();

        Serial.print("GYR: ");
        Serial.print("X:");
        Serial.print(qmc.getX());
        Serial.print(" Y:");
        Serial.print(qmc.getY());
        Serial.print(" Z:");
        Serial.print(qmc.getZ());
        Serial.println(" uT");
        Serial.print("RAW: ");
        Serial.print("X:");
        Serial.print(qmc.getRawX());
        Serial.print(" Y:");
        Serial.print(qmc.getRawY());
        Serial.print(" Z:");
        Serial.println(qmc.getRawZ());

        /*
        float x,  y,  z;
        qmc.getMag(x, y, z);
        Serial.print("X:");
        Serial.print(x);
        Serial.print(" Y:");
        Serial.print(y);
        Serial.print(" Z:");
        Serial.println(x);
        */
    }


    delay(100);
}



