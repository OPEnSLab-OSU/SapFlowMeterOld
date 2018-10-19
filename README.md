# Sap Flow Meter

This is the primary location of the Sap Flow Meter project code, which is a modified code of Project LOOM code by OPEnS Lab, and PCB designs, by using EAGLE CAD PCB designing tool.

## Table of Contents

1. [Installation](#installation)
2. [Device Support](#device-support)
3. [Communication Platforms](#communication-platforms)
4. [Device Configuration](#device-configuration)
	1. [Configuration File](#Configuration-File)
		1.[Enable sap flow meter](#Enable-sap-flow-meter)
		2.[HUB options](#HUB-options)
		3.[NODE options](#NODE-options)
		

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
| Google Sheets  | Google Sheets (PushingBox)* | Works as output, looking for ways of improving the pipline   |
| SD card        | **SD card**                 | Works as output, considering as input                        |
| Serial         | **Serial**                  | Works for output. Serial as input may or may not be useful (is how Fona is tested) |

## Device Configuration

The configuration of the code to upload and flash to devices is set in the config.h file. This file presents the options that can be set or toggled to achieve a particular behavior. The preprocessor uses these definitions to select the necessary declarations, variables, and functions automatically. 

Currently changes to the config need to be done manually in the file itself.

The configuration file is used in conjunction with preprocessor statements to essentially built the specificed sketch. Any options that can be set or toggled, or any specification of hardware (e.g. relay switch for HRM, and analog sensor for NTC thermistors) being used occur in (and only in) this config.h.

### Enable sap flow meter

Firstly, it is necessry to toggle "is_sapflow"

```cpp
// --- Prebuilt Devices ---
#define is_ishield      0	// 1 to specify using Ishield (should enable only wifi as communication platform)
#define is_multiplexer  0	// 1 if tca9548a Multiplexer is being used. (Further customization in advanced options)
#define is_sapflow      1 
#define is_evaporimeter 0
```

### Manipulate options

Set ether HUB or Node(probe meter)

### HUB options
- Set hub_node_type to be 0
- Set CHANNEL to be 1 - 8
- Set is_lora to be 1 if you want to communicate to receive data from node through LoRa. Default is 1.
- Set is_rtc to be 1 if you want to log timestamp
- Set is_sd to be 1 if you want to store recieved data in micro sd card
- If you want to rename excel file to be stored in the sd card, change char FileName[] = "file_name.CSV"; The file name should be short length to avoid any miscellenous error.

```cpp
// --- Sapflowmeter Options ---
#if is_sapflow == 1
	#define hub_node_type    0      // 0: hub, 1: node
  #define probe_type       0      // 0: TDM, 1: HRM
  #define sensor_type      1      // 0: NTC, 1: RTD
  #define CHANNEL          1      // Channel to use. Set to 1-8 for channels A-H, respectively. Alternatively can define to -1 to used advanced option INIT_INST
  #define REQUEST_SETTINGS 0      // 1 to request channel settings from Max Channel Manager, 0 to not
  #define AUTO_NAME        0      // 1 to enable naming device based on configured settings (if not set manual name in advanced options)
  #define is_lora          1      // enable LoRa
  #define is_rtc           1      // enable rtc
  #define is_sd            1      // enable microsd logger
   char FileName[]       =     "HUB1019.CSV";
```

- set is_ethernet to be 1 if you want to send data through ethernet
- set is_fona to be 1 if you want to send data through 2G network
- set is_pushingbox 1 if you want to store data in google sheet
- To store data in google sheet, it is necesary to define spreadsheet ID of google sheet that you want use and device ID. There is detail directions to setup pushingbox and googlesheet in the next section.

```cpp
  #if hub_node_type == 0       // if is hub
    #define is_ethernet   0
    #define is_fona       1
    #define is_pushingbox 1
    #if is_pushingbox == 1
      #define spreadsheet_id "17XjrTjXENChYsHMVvKgePg8rsohwz0hyASkdeZZKROk"   // This is Dongjun's Sapflow spreadsheet
      char device_id[]   =   "v4099B684340616E";
    #endif // of is_pushingbox
  #elif hub_node_type == 1     // if is node
  .
  .
  .
  ```
### NODE options
- Set hub_node_type to be 1
- Set probe_type to be 0 if you want TDM, or 1 if you want HRM
- Set sensor_type to be 0 if you want NTC thermister, 1 if you want RTD sensor. There will be more options for other sensors(eg. k type thermometer)
- Set CHANNEL to be 1 - 8
- Set is_lora to be 1 if you want to communicate to send data to HUB through LoRa. Default is 1.
- Set is_rtc to be 1 if you want to log timestamp
- Set is_sd to be 1 if you want to store measured data in micro sd card
- If you want to rename excel file to be stored in the sd card, change char FileName[] = "file_name.CSV"; The file name should be short length to avoid any miscellenous error.

```cpp
// --- Sapflowmeter Options ---
#if is_sapflow == 1
  #define hub_node_type    1      // 0: hub, 1: node
  #define probe_type       0      // 0: TDM, 1: HRM
  #define sensor_type      1      // 0: NTC, 1: RTD
  #define CHANNEL          1      // Channel to use. Set to 1-8 for channels A-H, respectively. Alternatively can define to -1 to used advanced option INIT_INST
  #define REQUEST_SETTINGS 0      // 1 to request channel settings from Max Channel Manager, 0 to not
  #define AUTO_NAME        0      // 1 to enable naming device based on configured settings (if not set manual name in advanced options)
  #define is_lora          1      // enable LoRa
  #define is_rtc           1      // enable rtc
  #define is_sd            1      // enable microsd logger
   char FileName[]       =     "TDM0731.CSV";
```

- Set is_sht31d to be 1 if you want to measure ambient temperature and humidity
- Set is_decagon to be 1 if you want to use soil moisture sensor
- If you use HRM probe, you can modify heat pulse duration. Default is 2.5 sec. The heater turns on for 2.5 sec and turn off for 2.5 sec repeatedly during heat duration.
- If you want to change data transmission interval, modify senddelay. 60000 for 1 min, 900000 for 15 min, 1800000 for 30 min. Default is 900000.
- If you want to change heat duration(Period that heater turns on),  change heatduration. Defualt for TDM is 240000(4 min), and for HRM is 120000 (2 min). The heater is turned on before data transmission time for the heat duration and turned off 1 min after data transmission. (For example, let's assume the code was initialized at 13:00. If senddelay is 15 min and heatduration is 4min, the heater is turned on at 13:11(4 mins before send interval). Probe meter measures and sends data at 13:15. The heater is turned off at 13:16. After this iteration, the heater is turned on again at 13:26. It sends data at 13:30. The heater is turned off at 13:31. Repeat this process until the battery is dead or probe meter is off)


```cpp
  .
  .
  .
  #elif hub_node_type == 1     // if is node
    #define num_analog    2      // two temperature sensors
    #define is_sht31d     1      // Ambient Temperature / Humidity
    #define is_decagon    1      // Soil moisture
    #define heatpulse     2500   // For HRM probe heat pulse (e.g 2500:2.5 sec) 
    #define senddelay     900000 // send data every 2500(=2.5s), 60000(=1m), 300000ms(=5m), 600000ms(=10m), 900000ms(=15m)
    #if probe_type == 0      // 0:TDM, 1: HRM
      #define heatduration  240000  // Turn on heater for 4 min
    #elif probe_type == 1
      #define heatduration  120000  // Turn on heater for 2 min
    #endif //of probe_type
  #endif // of hub_node_type
```
