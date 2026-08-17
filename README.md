# ESP32 FreeRTOS I2C Sensor Driver

A multi-threaded C++ application for ESP32 utilizing FreeRTOS tasks and queues to read data from an MPU6050 sensor via I2C.

## Features
- **OOP C++ Driver Design**: Custom encapsulation for I2C register operations.
- **Bitwise Byte Assembly**: Reconstructs 16-bit signed sensor values from 8-bit registers using bitwise shifts (`<< 8 |`).
- **FreeRTOS Multithreading**: Separate concurrent tasks for sensor polling and data logging.
- **Inter-Task Communication**: Thread-safe data transfer using FreeRTOS Queues (`xQueueSend` / `xQueueReceive`).

## Architecture
1. `sensorTask`: Reads temperature data from MPU6050 registers every 1000 ms and pushes it to `tempQueue`.
2. `displayTask`: Blocks until data arrives in `tempQueue`, then prints it to Serial output.

## Technologies Used
- **Language**: C++17
- **RTOS**: FreeRTOS
- **Protocol**: I2C
- **Hardware/Simulation**: ESP32, MPU6050 (Wokwi Simulator)
