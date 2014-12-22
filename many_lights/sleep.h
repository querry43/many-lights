#pragma once

#include "utils.h"

#include <avr/power.h>
#include <avr/sleep.h>

namespace sleep {

unsigned long _wakeup_millis = 0;

void _interrupt() {
  utils::debug(PCintPort::arduinoPin); // Serial prints are known to cause issues
};

void _attach_interrupt() {
  utils::debug("attach_interrupt()");
  for (unsigned int i = 0; i < config::num_input_pins(); i++)
    PCintPort::attachInterrupt(config::input_pins[i], &_interrupt, FALLING);
}

void _detach_interrupt() {
  utils::debug("detach_interrupt()");
  for (unsigned int i = 0; i < config::num_input_pins(); i++)
    PCintPort::detachInterrupt(config::input_pins[i]);
}

void _sleep() {
  utils::debug("sleep()", 1000); // the delay fixes serial prints that are not flushed before sleep

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  digitalWrite(config::pixel_pin, LOW);
  digitalWrite(config::power_enable_pin, HIGH);
  _attach_interrupt();
  sleep_mode();

  _detach_interrupt();
  _wakeup_millis = millis();
  digitalWrite(config::power_enable_pin, LOW);

  utils::debug("waking up from sleep");
}

void reset_sleep_timer() { _wakeup_millis = millis(); }

void update() {
  for (unsigned int i = 0; i < config::num_input_pins(); i++)
    if (digitalRead(config::input_pins[i]) == LOW)
      reset_sleep_timer();

  if (millis() - _wakeup_millis > config::sleep_after_millis)
    _sleep();
}

};
