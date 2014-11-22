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

void set_grid_pixel(int x, int y, int r, int g, int b) {
  _pixels.setPixelColor(y * config::num_grid_cols + x, r, g, b);
}

void set_grid_pixel(int x, int y, uint32_t c) {
  _pixels.setPixelColor(y * config::num_grid_cols + x, c);
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
      pixels::_pixels.setPixelColor(config::num_grid_pixels + i, _ring_color);
    else
      pixels::_pixels.setPixelColor(config::num_grid_pixels + i, 0, 0, 0);

    _pixels.show();
}

void clear() {
  _pixels.clear();
}

// pixels::burst(random(8), random(4), random(2), random(2), random(2));
void burst(int x, int y, int r, int g, int b) {
  bool inc = true;
  int t = 0;
  
  while (true) {
    int _r = t*r, _g = t*g, _b = t*b;

    pixels::set_grid_pixel(x, y, inc ? _r : max(0, _r-100), inc ? _g : max(0, _g-100), inc ? _b : max(0, _b-100));
    
    if (x < config::num_grid_cols - 1) pixels::set_grid_pixel(x+1, y, inc ? max(0, _r-100) : _r, inc ? max(0, _g-100) : _g, inc ? max(0, _b-100) : _b);
    if (x > 0) pixels::set_grid_pixel(x-1, y, inc ? max(0, _r-100) : _r, inc ? max(0, _g-100) : _g, inc ? max(0, _b-100) : _b);
    if (y < config::num_grid_rows - 1) pixels::set_grid_pixel(x, y+1, inc ? max(0, _r-100) : _r, inc ? max(0, _g-100) : _g, inc ? max(0, _b-100) : _b);
    if (y > 0) pixels::set_grid_pixel(x, y-1, inc ? max(0, _r-100) : _r, inc ? max(0, _g-100) : _g, inc ? max(0, _b-100) : _b);
    
    if (t >= 200)
      inc = false;

    else if (! inc && t <= 0) {
      clear();
      show();
      return;
    }
    
    if (inc)
      t++;
    else
      t--;
 
    show();
  }
}


};
