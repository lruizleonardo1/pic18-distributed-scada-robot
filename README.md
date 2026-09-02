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

## SPI Command Protocol

The master controller communicates with each slave using a dedicated chip-select line. Commands and data are transferred as SPI bytes.

| Command | Value | Target | Function |
|---|---:|---|---|
| `PAROO` | 0 | Motor slave | Stop the motors |
| `ULTRA` | 1 | Ultrasonic slave | Request the ultrasonic distance value |
| `MOTOR` | 2 | Motor slave | Trigger the target-position event |
| `RADAR` | 3 | Radar slave | Defined radar command |
| `FULLL` | 252 | Motor slave | Set maximum motor speed |
| `ATRAS` | 253 | Motor slave | Move backward |
| `ADELA` | 254 | Motor slave | Move forward |

Motor speed values are also transmitted as byte values and used by the motor slave to update the PWM duty cycle.

### Data Flow

1. A command is received by the master controller through UART/RS232.
2. The master requests a distance value from the ultrasonic slave through SPI.
3. The ultrasonic slave measures the echo pulse using Timer1 and places the scaled distance value in the SPI buffer.
4. The master receives the distance value and converts it into the working distance used by the positioning algorithm.
5. According to the difference between the measured distance and the selected target, the master sends direction and speed commands to the motor slave.
6. A scaled distance coordinate is transmitted to the radar slave for GLCD visualization.