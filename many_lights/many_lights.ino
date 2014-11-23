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
#include "pixels.h"
#include "sleep.h"
#include "utils.h"

int last_encoder_value;

pixels::random_burst bursts[] = {
  pixels::random_burst(config::red_button, 255, 0, 0),
  pixels::random_burst(config::green_button, 0, 255, 0),
  pixels::random_burst(config::blue_button, 0, 0, 255),
  pixels::random_burst(config::yellow_button, 255, 255, 0),
};

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

  for (int i = 0; i < 4; i++)
    bursts[i].update();
  
  pixels::show();
}

void _loop() {
  encoder::update();
  
  if (digitalRead(config::encoder_button) == HIGH)
    pixels::change_ring_color();
  for (int i = 0; i < 4; i++)
    bursts[i].update();
  
  encoder::set_color(1023, 0, 0);

  if (encoder::value() > last_encoder_value + config::encoder_sensitivity) {
    utils::debug("encoder clockwise");
    pixels::rotate_ring(true);
    last_encoder_value = encoder::value();
    sleep::reset_sleep_timer();
  } else if (encoder::value() < last_encoder_value - config::encoder_sensitivity) {
    utils::debug("encoder counter clockwise");
    pixels::rotate_ring(false);
    last_encoder_value = encoder::value();
    sleep::reset_sleep_timer();
  }
}
