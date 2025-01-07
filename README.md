# LoRaWAN Seminar Project

Welcome to the **LoRaWAN Seminar Project** repository! This project focuses on interfacing a DS18B20 temperature sensor with a LoRaWAN network, using a Dragino gateway for data transmission and visualization on The Things Network (TTN) dashboard.

## Repository Overview

This repository contains the following resources:

### Source Code
- **`main.c`**: Main application file for hardware initialization and LoRaWAN communication.
- **`LoraWAN2.h`**: Header file with utilities and definitions for LoRaWAN functions.
- **`ds18b20.c`** / **`ds18b20.h`**: Source and header files for interacting with the DS18B20 temperature sensor.

### Hardware
- **`hardware/LoraWAN_Platine_board.brd`**: PCB layout for the custom LoRaWAN hardware.
- **`hardware/LoraWAN_Platine_schematic.sch`**: Circuit schematic for hardware assembly.

## Prerequisites

To build and run this project, ensure you have the following:

### Software
- **Development Environment**: [Code Composer Studio Theia](https://www.ti.com/tool/CODECOMPOSER).

### Hardware Components
- **Development Board**: MSP430G2ET development kit (uses a MSP430G2553 microcontroller).
- **LoRaWAN Module**: Seeed Studio Wio-E5-LE-HF, mounted on a custom PCB (files in the `hardware` directory).
- **Temperature Sensor**: DS18B20, connected to the MSP430.
- **LoRaWAN Gateway**: Dragino LPS8N.

## Getting Started

### 1. Hardware Setup

1. Assemble the hardware using the provided PCB layout (**`LoraWAN_Platine_board.brd`**) and schematic (**`LoraWAN_Platine_schematic.sch`**).
2. Connect the DS18B20 temperature sensor and the Wio-E5 module as specified in the schematic.
3. Attach the custom PCB to the MSP430G2553 development kit.
4. Set up the Dragino LPS8N gateway:
   - Connect the gateway to the Internet.
   - Follow the [Dragino LPS8N User Manual](https://www.dragino.com/downloads/downloads/LoRa_Gateway/LPS8/LPS8_LoRaWAN_Gateway_User_Manual_v1.3.2.pdf) for configuration.

### 2. Software Setup

1. Open the project in **Code Composer Studio Theia**.
2. Configure the LoRaWAN credentials:
   - Retrieve the AppKey, DevID, and other parameters for your Wio-E5 module using AT commands.
   - Register the device and configure the IDs in your TTN dashboard. Change the AppKey in 'LoraWan2.c'.
   - Refer to the [Wio-E5 Module Documentation](https://wiki.seeedstudio.com/LoRa-E5_STM32WLE5JC_Module/) for details.
3. If you used different pins for the connection with the temperature sensor, update the pin configuration in 'LoraWan2.h'.
4. Confirm that the message identifiers used match the decoder on TTN. ('LoraWan2.h')
5. Compile the project.
6. Flash the compiled code to the MSP430 microcontroller.

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

### Hardware Files
- **`LoraWAN_Platine_board.brd`**: PCB layout for the custom LoRaWAN hardware.
- **`LoraWAN_Platine_schematic.sch`**: Schematic diagram for assembling the hardware.

## Contribution

This project was developed as part of a seminar at [**MedIT, RWTH Aachen University**](https://www.medit.hia.rwth-aachen.de/) under supervision of **Marian Walter**, by **Celina Helbing Vital** and **Oliver Schirrmacher**.

## Contact

For any questions or support, please contact the project maintainers at [**MedIT, RWTH Aachen University**](https://www.medit.hia.rwth-aachen.de/).

