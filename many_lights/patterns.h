#pragma once

#include "config.h"
#include "pixels.h"

namespace patterns {

class pattern {
protected:
  void merge_colors(int x, int y, int r, int g, int b) {
    pixels::set_grid_pixel(x, y, Adafruit_NeoPixel::Color(r, g, b) | pixels::get_grid_pixel(x, y));
  }

public:
  virtual void update() {
    if (digitalRead(config::encoder_button) == HIGH)
      this->reset();
  }

  virtual void reset() {
    pixels::clear();
  }
};

class wave : public pattern {
private:
  typedef enum {IDLE, ACTIVE} mode_t;
  mode_t mode;

  uint8_t pin;
  uint8_t _r, _g, _b;

  float index;
  int wave_width;

  uint8_t orientation;

  bool is_half_intensity(int int_index, int x, int y) {
    return x + y == int_index - 1
      || x + y == int_index - wave_width;
  }

  bool is_full_intensity(int int_index, int x, int y) {
    return x + y > int_index - wave_width;
  }

  bool is_off(int int_index, int x, int y) {
    return x + y == int_index - wave_width - 1;
  }

  void do_wave() {
    int int_index = (int) index;

    for (int x = 0; x < int_index; x++) {
      for (int y = 0; y < int_index - x; y++) {
        if (is_half_intensity(int_index, x, y))
          translate_pixel(x, y, _r * 0.5, _g * 0.5, _b * 0.5);
        else if (is_full_intensity(int_index, x, y))
          translate_pixel(x, y, _r, _g, _b);
        else if (is_off(int_index, x, y))
          translate_pixel(x, y, 0, 0, 0);
      }
    }

    index += 0.1;

    if (index > config::num_grid_cols + config::num_grid_rows + wave_width + 2)
      reset();
  }

  void translate_pixel(int x, int y, int r, int g, int b) {
    if (orientation % 2 == 1)
      x = map(x, 0, config::num_grid_cols, config::num_grid_cols, 0);

    if (orientation > 1)
      y = map(y, 0, config::num_grid_rows, config::num_grid_rows, 0);

    if (r == 0 && g == 0 && b == 0)
      pixels::set_grid_pixel(x, y, r, g, b);
    else
      merge_colors(x, y, r, g, b);
  }

  void check_button() {
    if (digitalRead(pin) == LOW) {
      mode = ACTIVE;
      orientation = random(0, 4);
    }
  }


public:
  wave(uint8_t pin, uint8_t r, uint8_t g, uint8_t b) : mode(IDLE), pin(pin), _r(r), _g(g), _b(b), index(0), wave_width(4) { }

  void update() {
    if (mode == IDLE)
      check_button();

    if (mode == ACTIVE)
      do_wave();

    pattern::update();
  }

  void reset() {
    mode = IDLE;
    index = 0;
    pattern::reset();
  }
};

class splotches : public pattern {
private:
  void draw(int r, int g, int b) {
    uint8_t x = random(config::num_grid_cols);
    uint8_t y = random(config::num_grid_rows);

    merge_colors(x, y, r, g, b);

    if (x < config::num_grid_cols - 1)
      merge_colors(x+1, y, r, g, b);
    if (x > 0)
      merge_colors(x-1, y, r, g, b);

    if (y < config::num_grid_rows - 1)
      merge_colors(x, y+1, r, g, b);
    if (y > 0)
      merge_colors(x, y-1, r, g, b);

    delay(100);
  }

public:
  void update() {
    if (digitalRead(config::green_button) == LOW)
      draw(0, 255, 0);
    if (digitalRead(config::red_button) == LOW)
      draw(255, 0, 0);
    if (digitalRead(config::blue_button) == LOW)
      draw(0, 0, 255);
    if (digitalRead(config::yellow_button) == LOW)
      draw(200, 255, 0);
    
    pattern::update();
  }
};

class theater_chase : public pattern {
private:
  int offset;

  void draw(uint32_t color) {
    for (int i = 0; i < config::num_grid_pixels; i += 3)
      pixels::set_grid_pixel(i+offset, 0);

    offset = (offset + 1) % 3;

    for (int i = 0; i < config::num_grid_pixels; i += 3)
      pixels::set_grid_pixel(i+offset, color);

    delay(50);
  }

public:
  theater_chase() : offset(0) { }

  void update() {
    uint32_t color = 0;

    if (digitalRead(config::green_button) == LOW)
      color |= Adafruit_NeoPixel::Color(0, 255, 0);
    if (digitalRead(config::red_button) == LOW)
      color |= Adafruit_NeoPixel::Color(255, 0, 0);
    if (digitalRead(config::blue_button) == LOW)
      color |= Adafruit_NeoPixel::Color(0, 0, 255);
    if (digitalRead(config::yellow_button) == LOW)
      color |= Adafruit_NeoPixel::Color(200, 255, 0);

    if (color == 0)
      reset();
    else
      draw(color);

    pattern::update();
  }
};

};

