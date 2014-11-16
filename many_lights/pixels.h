#pragma once

#include "config.h"

namespace pixels {

Adafruit_NeoPixel _pixels = Adafruit_NeoPixel(config::num_pixels, config::pixel_pin, NEO_GRB + NEO_KHZ800);

int _ring_values[config::num_ring_pixels];
uint32_t _ring_color;

void setup() {
  _pixels.begin();
  _pixels.show();

  for (int i = 0; i < config::num_ring_pixels; i++)
    _ring_values[i] = i % 4 == 0 ? 1 : 0;
  
  _ring_color = Adafruit_NeoPixel::Color(255, 0, 255);
}

void rotate_ring(boolean forward) {
  if (forward) {
    int last_value = _ring_values[config::num_ring_pixels-1];
    for (int i = 0; i < config::num_ring_pixels; i++) {
      int tmp = _ring_values[i];
      _ring_values[i] = last_value;
      last_value = tmp;
    }
  } else {
    int last_value = _ring_values[0];
    for (int i = config::num_ring_pixels-1; i >= 0; i--) {
      int tmp = _ring_values[i];
      _ring_values[i] = last_value;
      last_value = tmp;
    }
  }
}

unsigned long _last_change_millis;

void change_ring_color() {
  if (millis() - _last_change_millis > config::ring_change_delay) {
    _ring_color = Adafruit_NeoPixel::Color(
      random(10, 255),
      random(10, 255),
      random(10, 255)
    );
  
    _last_change_millis = millis();
  }
}

void show() {
  for (int i = 0; i < config::num_ring_pixels; i++)
    if (_ring_values[i])
      pixels::_pixels.setPixelColor(i, _ring_color);
    else
      pixels::_pixels.setPixelColor(i, 0, 0, 0);

    _pixels.show();
}

};
