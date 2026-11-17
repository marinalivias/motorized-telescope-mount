# Motorized Telescope Mount

Arduino motorization for the EQ3 mount of my telescope (Uranum Andrômeda II). The plan is to start with RA tracking and work up to a full GoTo system using OnStepX.

For now I'm prototyping in [Wokwi](https://wokwi.com) before building it with real parts.

## Prototype 1

One stepper motor on the RA axis with an A4988 driver and three buttons:

- green: move east
- blue: move west
- yellow: toggle sidereal tracking (red LED turns on)

To run it, create an Arduino Uno project on Wokwi, paste `sketch.ino` and `diagram.json` from the `prototype-1` folder and add the AccelStepper library.

## Roadmap

- [x] RA tracking in the simulator
- [ ] Second motor (DEC) with north/south buttons
- [ ] Breadboard build with NEMA 17 motors and TMC2209 drivers
- [ ] Fit it to the mount with GT2 belts
- [ ] GoTo with OnStepX + ESP32
