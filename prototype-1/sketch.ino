// Motorized Mount - prototype 1
// RA axis motor controlled by push buttons
//   pin 2 (green): moves east while held
//   pin 3 (blue): moves west while held
//   pin 4 (yellow): toggles sidereal tracking
//   pin 12 (red LED): on while tracking

#include <AccelStepper.h>

const int PIN_STEP = 9;
const int PIN_DIR = 8;
const int BTN_EAST = 2;
const int BTN_WEST = 3;
const int BTN_TRACKING = 4;
const int LED_TRACKING = 12;

// speeds in steps/s
const float MANUAL_SPEED = 400.0;

// sidereal rate on the real mount:
// 200 steps x 16 microsteps x (60/16) x 130 teeth = 1,560,000 steps per turn
// 1,560,000 / 86164 s = ~18.1 steps/s
// the simulated driver runs in full step mode, so it spins faster here
const float TRACKING_SPEED = 18.1;

AccelStepper motor(AccelStepper::DRIVER, PIN_STEP, PIN_DIR);

bool tracking = false;
bool lastTrackingState = HIGH;
unsigned long lastPress = 0;
const unsigned long DEBOUNCE_MS = 200;

void setup() {
  // INPUT_PULLUP uses the internal resistor, pressed button = LOW
  pinMode(BTN_EAST, INPUT_PULLUP);
  pinMode(BTN_WEST, INPUT_PULLUP);
  pinMode(BTN_TRACKING, INPUT_PULLUP);
  pinMode(LED_TRACKING, OUTPUT);

  motor.setMaxSpeed(1000);

  Serial.begin(115200);
  Serial.println("Motorized mount ready");
}

void loop() {
  checkTrackingButton();
  updateSpeed();

  // runSpeed has to be called all the time, no delay in the loop
  motor.runSpeed();
}

// each press on the yellow button toggles tracking
void checkTrackingButton() {
  bool state = digitalRead(BTN_TRACKING);

  bool justPressed = (state == LOW && lastTrackingState == HIGH);
  if (justPressed && millis() - lastPress > DEBOUNCE_MS) {
    tracking = !tracking;
    digitalWrite(LED_TRACKING, tracking ? HIGH : LOW);
    Serial.println(tracking ? "Tracking ON" : "Tracking OFF");
    lastPress = millis();
  }

  lastTrackingState = state;
}

// manual buttons take priority over tracking
void updateSpeed() {
  if (digitalRead(BTN_EAST) == LOW) {
    motor.setSpeed(MANUAL_SPEED);
  } else if (digitalRead(BTN_WEST) == LOW) {
    motor.setSpeed(-MANUAL_SPEED);
  } else if (tracking) {
    motor.setSpeed(TRACKING_SPEED);
  } else {
    motor.setSpeed(0);
  }
}
