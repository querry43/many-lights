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

class random_burst {
private:
  typedef enum {IDLE, INCREMENT, DECREMENT} mode_t;
  mode_t mode;

  uint8_t pin;
  uint8_t _r, _g, _b;
  uint8_t x, y;
  
  float t;
  
  random_burst() { }
  
  void check_button() {
    if (digitalRead(pin) == LOW) {
      mode = INCREMENT;
      x = random(config::num_grid_cols);
      y = random(config::num_grid_rows);
    }
  }
  
  void update_pattern() {
    if (mode == DECREMENT && t <= 0.1) {
      utils::debug("end of pattern");
      t = 0.0;
      mode = IDLE;
      draw(0, 0);
      
    } else {
      int r = t * _r,
          g = t * _g,
          b = t * _b;
      
      draw(
        Adafruit_NeoPixel::Color(mode == INCREMENT ? r : max(0, r-100), mode == INCREMENT ? g : max(0, g-100), mode == INCREMENT ? b : max(0, b-100)),
        Adafruit_NeoPixel::Color(mode == INCREMENT ? max(0, r-100) : r, mode == INCREMENT ? max(0, g-100) : g, mode == INCREMENT ? max(0, b-100) : b)
      );
  
      if (t >= 0.9)
        mode = DECREMENT;
      
      if (mode == INCREMENT)
        t += 0.01;
      else if (mode == DECREMENT)
        t -= 0.01;
    }
  }
  
  void draw(uint32_t interior_color, uint32_t exterior_color) {
    pixels::set_grid_pixel(x, y, interior_color);
    
    if (x < config::num_grid_cols - 1) 
      pixels::set_grid_pixel(x+1, y, exterior_color);
    if (x > 0)
      pixels::set_grid_pixel(x-1, y, exterior_color);

    if (y < config::num_grid_rows - 1)
      pixels::set_grid_pixel(x, y+1, exterior_color);
    if (y > 0)
      pixels::set_grid_pixel(x, y-1, exterior_color);
  }

public:
  random_burst(uint8_t pin, uint8_t r, uint8_t g, uint8_t b) : mode(IDLE), pin(pin), _r(r), _g(g), _b(b), t(0.0) { }
  
  void update() {
    if (mode == IDLE)
      check_button();
    
    if (mode != IDLE)
      update_pattern();
  }
};

};
