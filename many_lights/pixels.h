#pragma once

#include "config.h"

namespace pixels {

Adafruit_NeoPixel _pixels = Adafruit_NeoPixel(config::num_pixels, config::pixel_pin, NEO_GRB + NEO_KHZ800);

int _ring_values[config::num_ring_pixels];

void setup() {
  _pixels.begin();
  _pixels.show();

  for (int i = 0; i < config::num_ring_pixels; i++)
    _ring_values[i] = i % 4 == 0 ? 1 : 0;
}

void rotate_ring(int pos) {
  int last_value = _ring_values[config::num_ring_pixels-1];
  
  for (int i = 0; i < config::num_ring_pixels; i++) {
    int tmp = _ring_values[i];
    _ring_values[i] = last_value;
    last_value = tmp;
  }

}

};
