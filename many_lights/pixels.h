#pragma once

#include "config.h"

namespace pixels {

Adafruit_NeoPixel _pixels = Adafruit_NeoPixel(config::num_pixels, config::pixel_pin, NEO_GRB + NEO_KHZ800);

uint8_t _ring_color_index = 0;

void setup() {
  _pixels.begin();
}

void set_grid_pixel(int x, int y, int r, int g, int b) {
  _pixels.setPixelColor(y * config::num_grid_cols + x, r, g, b);
}

void set_grid_pixel(int x, int y, uint32_t c) {
  _pixels.setPixelColor(y * config::num_grid_cols + x, c);
}

void set_grid_pixel(int i, uint32_t c) {
  _pixels.setPixelColor(i, c);
}

void change_ring_color(bool forward) {
  if (forward)
    _ring_color_index = (_ring_color_index + 1) % config::num_ring_colors();
  else if (((signed) _ring_color_index) - 1 < 0)
    _ring_color_index =  config::num_ring_colors() - 1;
  else
    _ring_color_index = _ring_color_index - 1;
}


void show() {
  for (int i = 0; i < config::num_ring_pixels; i++)
    _pixels.setPixelColor(config::num_grid_pixels + i, config::ring_colors[_ring_color_index]);

  _pixels.show();
}

void clear() {
  _pixels.clear();
}

};
