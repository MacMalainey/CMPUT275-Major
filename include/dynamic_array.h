// Acknowledgement: this is based off the code provided in class for
// dynamic arrays.

#pragma once

#include <stdint.h>

template <typename T>
class DynamicArray {
 public:
  DynamicArray(uint16_t size = 0);
  ~DynamicArray();

  void insert(const T& item);
};
