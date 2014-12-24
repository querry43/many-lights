#pragma once

#include "config.h"
#include "encoder.h"

namespace pixels {

Adafruit_NeoPixel _pixels = Adafruit_NeoPixel(config::num_pixels, config::pixel_pin, NEO_GRB + NEO_KHZ800);

uint8_t _ring_color_index = 0;

void setup() {
  _pixels.begin();
  _pixels.setBrightness(config::pixel_brightness);
}

uint32_t index_from_coords(int x, int y) {
  return y * config::num_grid_cols + x;
}

uint32_t get_grid_pixel(int i) {
  return _pixels.getPixelColor(i);
}

uint32_t get_grid_pixel(int x, int y) {
  return get_grid_pixel(index_from_coords(x, y));
}

void set_grid_pixel(int i, uint32_t c) {
  if (i >= config::num_grid_pixels) return;
  _pixels.setPixelColor(i, c);
}

void set_grid_pixel(int x, int y, uint32_t c) {
  if (x < 0 || x >= config::num_grid_cols) return;
  if (y < 0 || y >= config::num_grid_rows) return;
  set_grid_pixel(index_from_coords(x, y), c);
}

void set_grid_pixel(int x, int y, int r, int g, int b) {
  _pixels.setPixelColor(index_from_coords(x, y), r, g, b);
}

void add_grid_color(int x, int y, int r, int g, int b) {
    set_grid_pixel(x, y, Adafruit_NeoPixel::Color(r, g, b) | get_grid_pixel(x, y));
}

void show() {
  for (int i = 0; i < config::num_ring_pixels; i++)
    _pixels.setPixelColor(config::num_grid_pixels + i, config::ring_colors[encoder::value()]);

  _pixels.show();
}

void clear() {
  _pixels.clear();
}

};
