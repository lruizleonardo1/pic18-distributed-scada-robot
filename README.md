# PIC18 Distributed SCADA Mobile Robot

Academic embedded-systems project developed as part of the Microsystems course.

The project implements a distributed SCADA architecture for a mobile robot using four PIC18 microcontrollers connected through SPI. The robot receives commands through a Bluetooth/RS232 interface and automatically moves toward or away from an obstacle until reaching a predefined target distance.

## System Architecture

The system is divided into one master controller and three dedicated slave controllers:

- **Master controller — PIC18F46K22**
  - Receives commands through UART/RS232.
  - Operates as SPI master.
  - Coordinates the ultrasonic, motor and radar nodes.
  - Implements the positioning logic.

- **Ultrasonic slave — PIC18F2550**
  - Measures obstacle distance using an ultrasonic sensor.
  - Uses Timer1 for echo pulse timing.
  - Sends the measured distance to the master through SPI.

- **Motor slave — PIC18F4550**
  - Controls two DC motors.
  - Receives direction and speed commands through SPI.
  - Generates PWM using CCP modules and Timer2.

- **Radar slave — PIC18F4550**
  - Receives distance information through SPI.
  - Displays the detected obstacle on a graphical LCD.
  - Implements a radar-style visualization.

## Communication

```text
Bluetooth / PC / Control Interface
              |
          UART / RS232
              |
        PIC18F46K22
         SPI Master
              |
      +-------+-------+
      |       |       |
 Ultrasonic  Motors  Radar
 PIC18F2550 PIC18F4550 PIC18F4550
      |       |       |
   Sensor    PWM     GLCD
