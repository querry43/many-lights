#pragma once

namespace config {

const unsigned long sleep_after_millis = 5 * 1000UL;

const int encoder_a = 6;
const int encoder_b = 7;
const int encoder_button = 8;
const int encoder_red = 9;
const int encoder_green = 10;
const int encoder_blue = 11;
const int encoder_sensitivity = 10;

const int power_enable_pin = 12;

const int num_grid_cols = 8;
const int num_grid_rows = 4;
const int num_ring_pixels = 16;
const int pixel_pin = 13;
const int num_grid_pixels = num_grid_cols * num_grid_rows;
const int num_pixels = num_grid_pixels + num_ring_pixels;


const int red_button = 2;
const int green_button = 3;
const int blue_button = 4;
const int yellow_button = 5;

const uint32_t ring_colors[] = {
  Adafruit_NeoPixel::Color(255,   0, 255),
  Adafruit_NeoPixel::Color(0,   255, 255),
};

const uint8_t num_ring_colors() {
  return sizeof(ring_colors) / sizeof(*ring_colors);
}

const int input_pins[] = {
  red_button,
  green_button,
  blue_button,
  yellow_button,
};

uint8_t num_input_pins() {
  return sizeof(input_pins) / sizeof(*input_pins);
}

};
