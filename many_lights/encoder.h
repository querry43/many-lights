#pragma once

#include "utils.h"

namespace encoder {
  
int _last_encoded;
int long _encoder_value;


int _get_value() {
  int MSB = digitalRead(config::encoder_a); //MSB = most significant bit
  int LSB = digitalRead(config::encoder_b); //LSB = least significant bit

  return (MSB << 1) | LSB; //converting the 2 pin value to single number
}

void setup() {
  pinMode(config::encoder_a, INPUT);
  pinMode(config::encoder_b, INPUT);

  digitalWrite(config::encoder_a, HIGH);
  digitalWrite(config::encoder_b, HIGH);
  
  pinMode(config::encoder_button, INPUT);
  
  pinMode(config::encoder_red, OUTPUT);
  pinMode(config::encoder_green, OUTPUT);
  pinMode(config::encoder_blue, OUTPUT);

  _last_encoded = _get_value();
}

void update() {
  int encoded = _get_value();
  int sum = (_last_encoded << 2) | encoded; //adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) _encoder_value++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) _encoder_value--;

  _last_encoded = encoded; //store this value for next time
}

int value() { return _encoder_value; }

void set_color(int r, int g, int b) {
  analogWrite(config::encoder_red, map(r, 0, 255, 1023, 0));
  analogWrite(config::encoder_green, map(g, 0, 255, 1023, 0));
  analogWrite(config::encoder_blue, map(b, 0, 255, 1023, 0));
}

}
