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
- The board raises its own WiFi access point and serves a touch
  control page — the robot is driven from a phone browser
- Quadrature encoders on both motors, counted by interrupt and readable
  live from the control page
- Closed-loop straight-line control: the two encoder counts are compared
  every 50 ms and the motors corrected to match

**Not yet built**
- Encoder on motor B
- IMU / stabilisation
- Chassis and sphere
- Audio and camera

## Hardware

| Part | Model |
|---|---|
| Controller | Arduino Nano ESP32 (ABX00092) |
| Motor driver | Pololu TB6612FNG carrier (#713) |
| Motors | 2 × Pololu 100:1 micro metal gearmotor HP 6V, extended shaft (#2214) |
| Encoders | Pololu magnetic encoder kit, side-entry JST SH (#4761), 12 CPR |
| Wheels | Pololu 60 × 8 mm, 3 mm D-shaft (#1423) |
| Regulator | Pololu S9V11F5S6CMA (#2870), SEL tied to OUT for 6 V |
| Battery | 2 × LG HJ2 18650 in a 2S holder |
| Switch | SPDT miniature toggle, wired as on/off |

## Pin assignment

The controller's header does **not** run in ascending order along the
breadboard: D12 sits at column 1 and the numbers descend from there.
Pin number and column number are different things — always wire by
column.

| MCU pin | Breadboard column | Function |
|---|---|---|
| D2 | 11 | STBY — driver enable |
| D3 | 10 | PWMA — motor A speed |
| D4 | 9 | AIN1 — motor A direction |
| D5 | 8 | AIN2 — motor A direction |
| D6 | 7 | PWMB — motor B speed |
| D7 | 6 | BIN1 — motor B direction |
| D8 | 5 | BIN2 — motor B direction |
| D9 | 4 | Encoder A, channel A |
| D10 | 3 | Encoder A, channel B |
| D11 | 2 | reserved — encoder B |
| D12 | 1 | reserved — encoder B |

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

## Encoders

The encoder board solders flat onto the two tabs on the back of the
motor, with the rear shaft passing through its centre hole. The
magnetic disc goes on **after** the board is soldered, pressed onto
the shaft from the outside, and must sit close to the board — the
field falls off fast, and a disc left a few millimetres out produces
no signal at all.

Motor power runs through the encoder board once fitted, so all six
lines share one JST SH cable:

| Wire | Function | Goes to |
|---|---|---|
| Red | Motor M1 | driver AO1, column 21 |
| Black | Motor M2 | driver AO2, column 22 |
| Blue | Sensor VCC | 3.3 V rail |
| Green | Sensor GND | ground rail |
| Yellow | Channel A | D9, column 4 |
| White | Channel B | D10, column 3 |

Resolution is 12 counts per motor-shaft revolution. With the 100:1
gearbox that is 1200 counts per wheel revolution, or about 6.4 counts
per millimetre travelled on a 60 mm wheel.

## Straight-line control

Two nominally identical motors do not turn at the same rate for the same
PWM command. Measured open-loop over three seconds, the two encoders
differed by about 20% — enough to send the robot in a slow curve rather
than a straight line.

The HTTP handlers therefore no longer drive the motors. They set a
target speed, and a control function running every 50 ms does the
driving: it reads both counters, works out how far each motor moved
since the last pass, takes the difference as the error, and applies a
proportional correction — slowing the leading motor and speeding up the
lagging one by the same amount, so the average speed is unchanged.

With a gain of 0.5 the same three-second test gives 5397 against 5047,
about 6.5% apart. A higher gain closes the gap further but overshoots
noticeably on start-up. Final tuning is deferred until the drive units
are mounted in the sphere, since the load will change the dynamics
entirely.

Timing uses `millis` rather than `delay`: the web server only checks for
requests when `handleClient` is called, so nothing in the main loop may
block.

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

Hold a drive button to move, release to stop. The encoder count
updates twice a second, and the dot beside the title lights while the
connection is alive.

## Code layout

The sketch is split across four `.ino` files. Arduino concatenates every
`.ino` in the folder into one translation unit before compiling — the
main file first, the rest alphabetically — so there are no includes
between them.

Two consequences worth knowing. Prototypes are only generated
automatically for functions defined in the main file, so anything moved
out needs a forward declaration added there by hand. And all globals
live in the main file, since it is always concatenated first and is
therefore the only place visible to every other file regardless of name.

- main — includes, pin constants, globals, forward declarations, setup, loop
- `motors.ino` — motor driving and the control loop
- `encoders.ino` — the two interrupt handlers
- `web.ino` — HTTP route handlers

## Repository

- `bb8_motor_control/` — early sketch, timed motor test sequence
- `bb8_wifi_drive/` — current firmware: WiFi AP, web server, drive
  control, encoder counting
