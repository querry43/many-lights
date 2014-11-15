#pragma once

namespace utils {

template <typename T>
void debug(T msg, int d = 0) {
#if 0
  Serial.println(msg);
  delay(d);
#endif
}

};
