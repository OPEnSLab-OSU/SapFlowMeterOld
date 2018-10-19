# Sap Flow Meter

This is the primary location of the Sap Flow Meter project code, which is a modified code of Project LOOM code by OPEnS Lab, and PCB designs, by using EAGLE CAD PCB designing tool.

## Table of Contents

1. [Installation](#installation)
2. [Device Support](#device-support)
3. [Communication Platforms](#communication-platforms)
4. [Device Configuration](#device-configuration)

## Installation

The Loom Library, and any dependencies, need to be placed into the Arduino libraries folder, i.e:

Document > Arduino > Libraries

## Device Support

- Arduino Feather M0

## Communication Platforms

WiFi and LoRa have had the most attention in development and thus have the most support and options, especially WiFi, as that is currently the only to communicate with a computer running Max/MSP. Other platforms have been implemented however. Below is the table of device inputs and outputs currently implemented, planned, or in consideration (bold for well implemented, * if partially implemented or in need of testing to be considered well implemented, plaintext if planned but not implemented, ? if in consideration). The top section is device-device communication platforms, and the bottom section are platforms that do not necessarily transmit directly to another Loom device

| Inputs         | Outputs                     | Comments                                                     |
| -------------- | --------------------------- | ------------------------------------------------------------ |
| **LoRa**       | **LoRa**                    | Implemented                                                  |                       
| Ethernet       | Ethernet*                   | Currently only used with hubs, expansion in progress         |
| GSM            | GSM*                        | Currently only one device, so outputs MQTT via GSM, but no GSM receiving yet |
| Google Sheets? | Google Sheets (PushingBox)* | Works as output, looking for ways of improving the pipline   |
| SD card?       | **SD card**                 | Works as output, considering as input                        |
| Serial?        | **Serial**                  | Works for output. Serial as input may or may not be useful (is how Fona is tested) |
