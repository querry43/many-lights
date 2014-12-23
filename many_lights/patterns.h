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
      pixels::add_grid_color(x, y, r, g, b);
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
  }

  void reset() {
    mode = IDLE;
    index = 0;
  }
};

class splotches {
private:
  void draw(int r, int g, int b) {
    uint8_t x = random(config::num_grid_cols);
    uint8_t y = random(config::num_grid_rows);

    pixels::add_grid_color(x, y, r, g, b);

    if (x < config::num_grid_cols - 1)
      pixels::add_grid_color(x+1, y, r, g, b);
    if (x > 0)
      pixels::add_grid_color(x-1, y, r, g, b);

    if (y < config::num_grid_rows - 1)
      pixels::add_grid_color(x, y+1, r, g, b);
    if (y > 0)
      pixels::add_grid_color(x, y-1, r, g, b);

    delay(100);
  }

public:
  void update() {
    for (int i = 0; i < config::num_grid_pixels; i++) {
      uint32_t color = pixels::get_grid_pixel(i);
      uint8_t r = (uint8_t)(color >> 16);
      uint8_t g = (uint8_t)(color >> 8);
      uint8_t b = (uint8_t)color;
      pixels::set_grid_pixel(i, Adafruit_NeoPixel::Color(max(r-1, 0), max(g-1, 0), max(b-1, 0)));
    }

    if (digitalRead(config::green_button) == LOW)
      draw(0, 255, 0);
    if (digitalRead(config::red_button) == LOW)
      draw(255, 0, 0);
    if (digitalRead(config::blue_button) == LOW)
      draw(0, 0, 255);
    if (digitalRead(config::yellow_button) == LOW)
      draw(200, 255, 0);
  }
};

class theater_chase {
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
      pixels::clear();
    else
      draw(color);
  }
};

class heart {
private:
  uint32_t color;
  float hue;

  void draw() {
    pixels::set_grid_pixel(2, 0, color);
    pixels::set_grid_pixel(4, 0, color);

    for (int x = 1; x <= 5; x++)
      pixels::set_grid_pixel(x, 1, color);

    for (int x = 1; x <= 5; x++)
      pixels::set_grid_pixel(x, 1, color);

    for (int x = 2; x <= 4; x++)
      pixels::set_grid_pixel(x, 2, color);

    pixels::set_grid_pixel(3, 3, color);
  }

  // This function taken from here:
  // http://eduardofv.com/read_post/179-Arduino-RGB-LED-HSV-Color-Wheel-
  void update_color() {
    unsigned int inSaturation = 255;
    unsigned int inBrightness = 255;

    unsigned int scaledHue = (hue * 6);
    unsigned int sector = scaledHue >> 8; // sector 0 to 5 around the color wheel
    unsigned int offsetInSector = scaledHue - (sector << 8);  // position within the sector
    unsigned int p = (inBrightness * ( 255 - inSaturation )) >> 8;
    unsigned int q = (inBrightness * ( 255 - ((inSaturation * offsetInSector) >> 8) )) >> 8;
    unsigned int t = (inBrightness * ( 255 - ((inSaturation * ( 255 - offsetInSector )) >> 8) )) >> 8;

    int r, g, b;

    switch( sector ) {
      case 0:
        r = inBrightness;
        g = t;
        b = p;
        break;
      case 1:
        r = q;
        g = inBrightness;
        b = p;
        break;
      case 2:
        r = p;
        g = inBrightness;
        b = t;
        break;
      case 3:
        r = p;
        g = q;
        b = inBrightness;
        break;
      case 4:
        r = t;
        g = p;
        b = inBrightness;
        break;
      default:    // case 5:
        r = inBrightness;
        g = p;
        b = q;
        break;
    }

    color = Adafruit_NeoPixel::Color(r, g, b);

    hue = fmod(hue + 0.1, 256);
  }

public:
  heart() : hue(100.0) { update_color(); }

  void update() {
    update_color();
    draw();
  };
};

};

