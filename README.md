# Motorized Telescope Mount

Arduino motorization for the EQ3 mount of my telescope (Uranum Andrômeda II). The plan is to start with RA tracking and work up to a full GoTo system using OnStepX.

For now I'm prototyping in [Wokwi](https://wokwi.com) before building it with real parts.

To run a prototype, create an Arduino Uno project on Wokwi, paste the `sketch.ino` and `diagram.json` from its folder and add the AccelStepper library.

## Prototype 1

One stepper motor on the RA axis with an A4988 driver and three buttons:

- green: move east
- blue: move west
- yellow: toggle sidereal tracking (red LED turns on)

## Prototype 2

Both axes, each with its own A4988 driver, and six buttons:

- green / blue: east / west (RA)
- white / grey: north / south (DEC)
- yellow: toggle sidereal tracking (RA only, DEC stays still)
- red: switch between slow speed for centering and fast speed for moving around

One LED shows tracking and the other shows fast mode.

## Roadmap

- [x] RA tracking in the simulator
- [x] Second motor (DEC) with north/south buttons and speed control
- [ ] Breadboard build with NEMA 17 motors and TMC2209 drivers
- [ ] Fit it to the mount with GT2 belts
- [ ] GoTo with OnStepX + ESP32
