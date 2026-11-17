// Motorized Mount - prototype 2
// RA and DEC motors with manual control and RA tracking
//   pin 2 (green): east, RA
//   pin 3 (blue): west, RA
//   pin 4 (white): north, DEC
//   pin 5 (grey): south, DEC
//   pin 10 (yellow): toggles sidereal tracking
//   pin 11 (red): toggles slow/fast manual speed
//   pin 12 (red LED): on while tracking
//   pin 13 (red LED): on in fast mode

#include <AccelStepper.h>

const int RA_STEP = 9;
const int RA_DIR = 8;
const int DEC_STEP = 7;
const int DEC_DIR = 6;

const int BTN_EAST = 2;
const int BTN_WEST = 3;
const int BTN_NORTH = 4;
const int BTN_SOUTH = 5;
const int BTN_TRACKING = 10;
const int BTN_SPEED = 11;

const int LED_TRACKING = 12;
const int LED_FAST = 13;

// speeds in steps/s
const float SLOW_SPEED = 50.0;   // fine centering at the eyepiece
const float FAST_SPEED = 600.0;  // moving across the sky
const float TRACKING_SPEED = 18.1;

const unsigned long DEBOUNCE_MS = 200;

AccelStepper ra(AccelStepper::DRIVER, RA_STEP, RA_DIR);
AccelStepper dec(AccelStepper::DRIVER, DEC_STEP, DEC_DIR);

bool tracking = false;
bool fastMode = false;

// toggle buttons need to remember their last state
struct ToggleButton {
  int pin;
  bool lastState;
  unsigned long lastPress;
};

ToggleButton trackingBtn = {BTN_TRACKING, HIGH, 0};
ToggleButton speedBtn = {BTN_SPEED, HIGH, 0};

// true only at the moment the button goes down
bool wasPressed(ToggleButton &btn) {
  bool state = digitalRead(btn.pin);
  bool pressed = (state == LOW && btn.lastState == HIGH && millis() - btn.lastPress > DEBOUNCE_MS);
  if (pressed) {
    btn.lastPress = millis();
  }
  btn.lastState = state;
  return pressed;
}

bool isHeld(int pin) {
  return digitalRead(pin) == LOW;
}

float manualSpeed() {
  return fastMode ? FAST_SPEED : SLOW_SPEED;
}

// manual buttons take priority over tracking
void updateRA() {
  if (isHeld(BTN_EAST)) {
    ra.setSpeed(manualSpeed());
  } else if (isHeld(BTN_WEST)) {
    ra.setSpeed(-manualSpeed());
  } else if (tracking) {
    ra.setSpeed(TRACKING_SPEED);
  } else {
    ra.setSpeed(0);
  }
}

// on an equatorial mount DEC stays still while tracking, it only moves by hand
void updateDEC() {
  if (isHeld(BTN_NORTH)) {
    dec.setSpeed(manualSpeed());
  } else if (isHeld(BTN_SOUTH)) {
    dec.setSpeed(-manualSpeed());
  } else {
    dec.setSpeed(0);
  }
}

void setup() {
  // INPUT_PULLUP uses the internal resistor, pressed button = LOW
  pinMode(BTN_EAST, INPUT_PULLUP);
  pinMode(BTN_WEST, INPUT_PULLUP);
  pinMode(BTN_NORTH, INPUT_PULLUP);
  pinMode(BTN_SOUTH, INPUT_PULLUP);
  pinMode(BTN_TRACKING, INPUT_PULLUP);
  pinMode(BTN_SPEED, INPUT_PULLUP);
  pinMode(LED_TRACKING, OUTPUT);
  pinMode(LED_FAST, OUTPUT);

  ra.setMaxSpeed(1000);
  dec.setMaxSpeed(1000);

  Serial.begin(115200);
  Serial.println("Motorized mount ready (speed: SLOW)");
}

void loop() {
  if (wasPressed(trackingBtn)) {
    tracking = !tracking;
    digitalWrite(LED_TRACKING, tracking ? HIGH : LOW);
    Serial.println(tracking ? "Tracking ON" : "Tracking OFF");
  }

  if (wasPressed(speedBtn)) {
    fastMode = !fastMode;
    digitalWrite(LED_FAST, fastMode ? HIGH : LOW);
    Serial.println(fastMode ? "Speed: FAST" : "Speed: SLOW");
  }

  updateRA();
  updateDEC();

  // both motors need runSpeed all the time, no delay in the loop
  ra.runSpeed();
  dec.runSpeed();
}
