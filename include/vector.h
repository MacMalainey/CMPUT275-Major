/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#pragma once

// Custom vector class to help not use double pointers and heap allocations
// everywhere

template <typename T>
class Vector {
  T *array;
  size_t capacity;
  size_t count;

  const size_t DEFAULT_CAPACITY = 10;

 public:
  Vector();
  Vector(size_t size);

  void Push(const T &value);
  T &Get();              // Get first item
  T &Get(size_t index);  // Get random item

  T &Pop();  // Get last item

  size_t Size();

  size_t Capacity();

  T *Begin();
  T *End();
};

template <typename T>
Vector<T>::Vector() : Vector(DEFAULT_CAPACITY) {}

template <typename T>
Vector<T>::Vector(size_t size) {
  capacity = size;
  array = new T[capacity];
  count = 0;
}

// Always makes a copy!
template <typename T>
void Vector<T>::Push(const T &value) {
  if (count == capacity) {
    // Make new temp array
    T *tempArr = new T[2 * capacity];

    // Copy all data
    for (size_t i = 0; i < capacity; i++) {
      tempArr[i] = array[i];
    }

    // Clean up old array
    delete[] array;
    capacity *= 2;
    array = tempArr;
  }
  array[count] = value;
  count++;
}

template <typename T>
T &Vector<T>::Get() {
  return Get(0);
}

template <typename T>
T &Vector<T>::Get(size_t index) {
  if (index >= 0 && index < capacity) {
    return array[index];
  }
}

template <typename T>
T &Vector<T>::Pop() {
  return array[count--];
}

template <typename T>
size_t Vector<T>::Size() {
  return count;
}

template <typename T>
size_t Vector<T>::Capacity() {
  return capacity;
}

template <typename T>
T *Vector<T>::Begin() {
  return array;
}

template <typename T>
T *Vector<T>::End() {
  return *(array[count]);
}
