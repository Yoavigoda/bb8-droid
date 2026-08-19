# BB-8 Droid

A self-built BB-8 style rolling ball droid: a drive unit inside a
transparent sphere, with a magnetically-held head on top.
Built from scratch — electronics, firmware and mechanics.

![Breadboard wiring of the drive electronics](wiring.jpg)

## Current status

**Working**
- Two micro metal gearmotors driven through a TB6612FNG H-bridge
- Regulated 6 V motor supply from a 2S 18650 pack
- Master power toggle
- The board raises its own WiFi access point and serves a control
  page — the robot is driven from a phone browser

**Not yet built**
- Wheel encoders (hardware in hand, not installed)
- IMU / stabilisation
- Chassis and sphere
- Audio and camera

## Hardware

| Part | Model |
|---|---|
| Controller | Arduino Nano ESP32 (ABX00092) |
| Motor driver | Pololu TB6612FNG carrier (#713) |
| Motors | 2 × Pololu 100:1 micro metal gearmotor HP 6V, extended shaft (#2214) |
| Encoders | Pololu magnetic encoder kit, JST SH (#4761) — not yet fitted |
| Wheels | Pololu 60 × 8 mm, 3 mm D-shaft (#1423) |
| Regulator | Pololu S9V11F5S6CMA (#2870), SEL tied to OUT for 6 V |
| Battery | 2 × LG HJ2 18650 in a 2S holder |
| Switch | SPDT miniature toggle, wired as on/off |

## Pin assignment

| MCU pin | Function |
|---|---|
| D2 | STBY — driver enable |
| D3 | PWMA — motor A speed |
| D4 / D5 | AIN1 / AIN2 — motor A direction |
| D6 | PWMB — motor B speed |
| D7 / D8 | BIN1 / BIN2 — motor B direction |
| D9–D12 | reserved for encoders |
| A4 / A5 | reserved for IMU (I2C) |

## Breadboard layout (830-point board)

| Columns | Component |
|---|---|
| 1–15 | Nano ESP32, straddling the centre channel |
| 19–26 | TB6612FNG, straddling the centre channel |
| 42–46 | S9V11F5S6CMA regulator, row a |
| 50–52 | Power toggle |

Driver pins on the a–e side, by column:
19 GND · 20 VCC · 21 AO1 · 22 AO2 · 23 BO2 · 24 BO1 · 25 VMOT · 26 GND

Logic rails (a–e side) carry 3.3 V from the Nano.
Motor power enters at VMOT, column 25, from the regulator OUT at column 42.

## Motor directions

Measured with both motors viewed from the same side, front shafts
facing the observer, on the `forward` command:

- Motor A (columns 21/22): clockwise
- Motor B (columns 23/24): counter-clockwise

They turn opposite ways on the bench. This is intended — inside the
sphere the two drive units face each other, rotated 180°, so opposite
bench directions push the shell the same way.

## Running it

1. Power the robot with the toggle switch
2. Connect a phone to the `BB8-Droid` WiFi network
3. Open `192.168.4.1` in a browser

## Repository

- `bb8_motor_control/` — early sketch, timed motor test sequence
- `bb8_wifi_drive/` — current firmware: WiFi AP, web server, drive control
