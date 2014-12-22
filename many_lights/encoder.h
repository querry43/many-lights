#pragma once

#include "sleep.h"
#include "utils.h"

namespace encoder {

int _last_encoded;
int long _encoder_value;
int long _last_encoder_value;
int _ring_setting = 0;


int _get_value() {
  int MSB = digitalRead(config::encoder_a); //MSB = most significant bit
  int LSB = digitalRead(config::encoder_b); //LSB = least significant bit

  return (MSB << 1) | LSB; //converting the 2 pin value to single number
}

void setup() {
  pinMode(config::encoder_a, INPUT);
  pinMode(config::encoder_b, INPUT);

  digitalWrite(config::encoder_a, HIGH);
  digitalWrite(config::encoder_b, HIGH);

  pinMode(config::encoder_button, INPUT);

  pinMode(config::encoder_red, OUTPUT);
  pinMode(config::encoder_green, OUTPUT);
  pinMode(config::encoder_blue, OUTPUT);

  _last_encoded = _get_value();
}

void change_ring_setting(bool inc) {
  _last_encoder_value = _encoder_value;
  sleep::reset_sleep_timer();

  if (inc)
    _ring_setting = (_ring_setting + 1) % config::num_ring_colors();
  else if (_ring_setting - 1 < 0)
    _ring_setting =  config::num_ring_colors() - 1;
  else
    _ring_setting = _ring_setting - 1;

  utils::debug("encoder change");
}

int value() { return _ring_setting; }

void update() {
  int encoded = _get_value();
  int sum = (_last_encoded << 2) | encoded; //adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) _encoder_value++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) _encoder_value--;

  _last_encoded = encoded; //store this value for next time

  if (_encoder_value > _last_encoder_value + config::encoder_sensitivity)
    change_ring_setting(true);
  else if (_encoder_value < _last_encoder_value - config::encoder_sensitivity)
    change_ring_setting(false);
}

void set_color(int r, int g, int b) {
  analogWrite(config::encoder_red, map(r, 0, 255, 1023, 0));
  analogWrite(config::encoder_green, map(g, 0, 255, 1023, 0));
  analogWrite(config::encoder_blue, map(b, 0, 255, 1023, 0));
}

}
