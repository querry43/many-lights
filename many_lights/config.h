#pragma once

namespace config {

const unsigned long sleep_after_millis = 5 * 1000UL;

const int encoder_a = 6;
const int encoder_b = 7;
const int encoder_button = 8;
const int encoder_red = 9;
const int encoder_green = 10;
const int encoder_blue = 11;
const int encoder_sensitivity = 3;

const int power_enable_pin = 12;

const int num_grid_cols = 8;
const int num_grid_rows = 4;
const int num_ring_pixels = 16;
const int pixel_pin = 13;
const int num_grid_pixels = num_grid_cols * num_grid_rows;
const int num_pixels = num_grid_pixels + num_ring_pixels;

const unsigned long ring_change_delay = 250UL;

const int red_button = 2;
const int green_button = 3;
const int blue_button = 4;
const int yellow_button = 5;

const int input_pins[] = {
  red_button,
  green_button,
  blue_button,
  yellow_button,
};

unsigned int num_input_pins() {
  return sizeof(input_pins)/sizeof(*input_pins);
}

};
