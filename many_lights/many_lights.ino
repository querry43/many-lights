/* this idea appears sound and could work
   remaining functionality:
     individual buttons add a random pixel of button color
     encoder cycles existing leds through color wheel
     encoder also rotates led ring
     encoder button does random color pulse
     any button presses will cause wakeup and reset the wakeup timer
   consider:
     using 3xAA batteries (instead of 4) to avoid needing a regulator
   questions:
     will the sleep power draw be low enough? best: 124u
     maybe disable brownout protection or external oscillator for better power usage?
   references:
     http://bildr.org/2012/08/rotary-encoder-arduino/
     https://www.sparkfun.com/tutorials/309
     http://www.nongnu.org/avr-libc/user-manual/group__avr__power.html
*/


// the arduino IDE needs libraries loaded first
#include <Adafruit_NeoPixel.h>
#include <PinChangeInt.h>

#include "config.h"
#include "encoder.h"
#include "patterns.h"
#include "pixels.h"
#include "sleep.h"
#include "utils.h"


patterns::wave waves[] = {
  patterns::wave(config::red_button, 255, 0, 0),
  patterns::wave(config::green_button, 0, 255, 0),
  patterns::wave(config::blue_button, 0, 0, 255),
  patterns::wave(config::yellow_button, 200, 255, 0),
};

patterns::theater_chase theater_chase;
patterns::splotches splotches;
patterns::heart heart;

void setup() {
  Serial.begin(9600);
  utils::debug("setup()");

  encoder::setup();
  pixels::setup();

  for (unsigned int i = 0; i < config::num_input_pins(); i++) {
    pinMode(config::input_pins[i], INPUT);
    digitalWrite(config::input_pins[i], HIGH);
  }

  pinMode(config::power_enable_pin, OUTPUT);
  digitalWrite(config::power_enable_pin, LOW);
}

void loop() {
  sleep::update();
  encoder::update();

  switch (encoder::value()) {
    case 0:
      splotches.update();
      break;
    case 1:
      for (int i = 0; i < 4; i++) waves[i].update();
      break;
    case 2:
      heart.update();
      break;
    case 3:
      theater_chase.update();
      break;
  }

  pixels::show();
}
