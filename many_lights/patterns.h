#pragma once

#include "config.h"
#include "pixels.h"

namespace patterns {

class wave {
private:
  typedef enum {IDLE, ACTIVE} mode_t;
  mode_t mode;

  uint8_t pin;
  uint8_t _r, _g, _b;

  float index;
  int wave_width;

  bool is_half_intensity(int x, int y) {
    return x + y == index - 1
      || x + y == (int) index - wave_width;
  }

  bool is_full_intensity(int x, int y) {
    return x + y > (int) index - wave_width;
  }

  bool is_off(int x, int y) {
    return x + y == (int) index - wave_width - 1;
  }

  void do_wave() {
    for (int x = 0; x < (int) index; x++) {
      for (int y = 0; y < (int) index - x; y++) {
        if (is_half_intensity(x, y))
          pixels::set_grid_pixel(x, y, _r * 0.5, _g * 0.5, _b * 0.5);
        else if (is_full_intensity(x, y))
          pixels::set_grid_pixel(x, y, _r, _g, _b);
        else if (is_off(x, y))
          pixels::set_grid_pixel(x, y, 0, 0, 0);
      }
    }

    index += 0.1;

    if (index > config::num_grid_cols + config::num_grid_rows + wave_width)
      reset();
  }

  void check_button() {
    if (digitalRead(pin) == LOW)
      mode = ACTIVE;
  }

public:
  wave(uint8_t pin, uint8_t r, uint8_t g, uint8_t b) : mode(IDLE), pin(pin), _r(r), _g(g), _b(b), index(0), wave_width(4) { }

  void update() {
    if (mode == IDLE)
      check_button();

    if (mode == ACTIVE)
      do_wave();
  }

  void reset() {
    mode = IDLE;
    index = 0;
  }
};

class random_burst {
private:
  typedef enum {IDLE, INCREMENT, DECREMENT} mode_t;
  mode_t mode;

  uint8_t pin;
  uint8_t _r, _g, _b;
  uint8_t x, y;

  float t;

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

  void reset() {
    mode = IDLE;
    t = 0.0;
  }
};

const int chaser_cycles = 5;

// modified from the adafruit sample code
void theater_chase(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  uint32_t color = Adafruit_NeoPixel::Color(r, g, b);
  
  for (int cycle = 0;  cycle < chaser_cycles; cycle++) {
    for (int  offset = 0; offset < 3; offset++) {
      for (int i = 0; i < config::num_grid_pixels; i += 3)
        pixels::set_grid_pixel(i+offset, color);

      pixels::show();
      delay(wait);
     
      for (int i = 0; i < config::num_grid_pixels; i += 3)
        pixels::set_grid_pixel(i+offset, 0);
    }
  }
}

};

