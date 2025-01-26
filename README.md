# LoRaWAN Seminar Project

Welcome to the **LoRaWAN Seminar Project** repository! This project focuses on interfacing a DS18B20 temperature sensor with a LoRaWAN network, using a Dragino gateway for data transmission and visualization on The Things Network (TTN) dashboard.

## Repository Overview

This repository contains the following resources:

### Source Code
- **`main.c`**: Main application file for hardware initialization and LoRaWAN communication.
- **`LoraWAN2.h`**: Header file with utilities and definitions for LoRaWAN functions.
- **`ds18b20.c`** / **`ds18b20.h`**: Source and header files for interacting with the DS18B20 temperature sensor.
- **`payloaddecoder.js`**: Uplink Payload decoder used in TTN.

### Hardware
- **`hardware/LoraWAN_Platine_board.brd`**: PCB layout for the custom LoRaWAN hardware.
- **`hardware/LoraWAN_Platine_schematic.sch`**: Circuit schematic for hardware assembly.
- **`hardware/3D Case MSP430 LoraWAN V2.f3d`**: Fusion file containing the design of the custom case for 3D printing.

## Prerequisites

To build and run this project, ensure you have the following:

### Software
- **Development Environment**: [Code Composer Studio Theia](https://www.ti.com/tool/CODECOMPOSER).

### Hardware Components
- **Development Board**: MSP430G2ET development kit (uses a MSP430G2553 microcontroller).
- **LoRaWAN Module**: Seeed Studio Wio-E5-LE-HF, mounted on a custom PCB (files in the `hardware` directory).
- **Temperature Sensor**: DS18B20, connected to the MSP430.
- **LoRaWAN Gateway**: Dragino LPS8N.
- **Case**: 3D printable Case.

## Getting Started

### 1. Hardware Setup

1. Assemble the hardware using the provided PCB layout (**`LoraWAN_Platine_board.brd`**) and schematic (**`LoraWAN_Platine_schematic.sch`**).
2. Connect the DS18B20 temperature sensor and the Wio-E5 module as specified in the schematic.
3. Attach the custom PCB to the MSP430G2553 development kit.
4. Set up the Dragino LPS8N gateway:
   - Connect the gateway to the Internet.
   - Follow the [Dragino LPS8N User Manual](https://www.dragino.com/downloads/downloads/LoRa_Gateway/LPS8/LPS8_LoRaWAN_Gateway_User_Manual_v1.3.2.pdf) for configuration.
5. Print the top and button part of the Case in **`3D Case MSP430 LoraWAN V2.f3d`** and place the MSP430 board including the custom PCB in it.

### 2. Software Setup

1. Open the project in **Code Composer Studio Theia**.
2. Configure the LoRaWAN credentials:
   - Retrieve the DevID and other parameters for your Wio-E5 module using AT commands. Also set the AppKey.
   - Register the device and configure the IDs in your TTN dashboard. Change the AppKey in 'LoraWan2.c'.
   - Refer to the [Wio-E5 Module Documentation](https://wiki.seeedstudio.com/LoRa-E5_STM32WLE5JC_Module/) for details.
3. If you used different pins for the connection with the temperature sensor, update the pin configuration in 'LoraWan2.h'.
4. Confirm that the message identifiers used match the decoder on TTN. ('LoraWan2.h')
   - If not change your device's uplink payload formatter in the TTN to the one in **`payloaddecoder.js`**.
6. Compile the project.
7. Flash the compiled code to the MSP430 microcontroller.

### 3. Running the Application

1. Power on the hardware.
2. The application will:
   - Initialize the hardware.
   - Read temperature data from the DS18B20 sensor.
   - Transmit the temperature data via LoRaWAN at regular intervals.
3. View the transmitted data in real-time on the TTN live data console.

## File Descriptions

### Source Code
- **`main.c`**: Core logic for the application, including hardware initialization and LoRaWAN communication.
- **`LoraWAN2.h`**: Definitions and utility functions for LoRaWAN functionality.
- **`ds18b20.c`** / **`ds18b20.h`**: Code for reading temperature data from the DS18B20 sensor.
- **`payloaddecoder.js`**: Uplink Payload decoder used in TTN.

### Hardware Files
- **`LoraWAN_Platine_board.brd`**: PCB layout for the custom LoRaWAN hardware.
- **`LoraWAN_Platine_schematic.sch`**: Schematic diagram for assembling the hardware.
- **`hardware/3D Case MSP430 LoraWAN V2.f3d`**: Fusion file containing the design of the custom case for 3D printing.

## Contribution

This project was developed as part of a seminar at [**MedIT, RWTH Aachen University**](https://www.medit.hia.rwth-aachen.de/) under supervision of **Marian Walter**, by **Celina Helbing Vital** and **Oliver Schirrmacher**.

## Contact

For any questions or support, please contact the project maintainers at [**MedIT, RWTH Aachen University**](https://www.medit.hia.rwth-aachen.de/).

## Copyright
If code from other sources has been used, the respective copyright notices are included directly in the code files.
The file **`hardware/3D Case MSP430 LoraWAN V2.f3d`** was created using a modified version of the template from [Thingiverse](https://www.thingiverse.com/thing:4651831). The original design was made by **JOHNeMac36** and is licensed under the **GNU General Public License (GPL)**.

