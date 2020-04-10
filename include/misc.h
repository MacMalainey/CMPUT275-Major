/**
 * CMPUT 275 -- Winter 2020 -- Final Project
 *
 * Mackenzie Malainey, 1570494
 * Benjamin Kong, 1573684
 * Lora Ma, 1570935
 * Michael Kwok, 1548454
 */

#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "vector.h"

struct Point {
  uint16_t x;
  uint16_t y;

  // Equality operator.
  bool operator==(const Point &other) const {
    return (this->x == other.x) && (this->y == other.y);
  }

  // Manhanttan distance between two points.
  uint16_t operator-(const Point &other) const {
    return (abs(this->x - other.x) + abs(this->y - other.y));
  }
};

enum Orientation { NORTH = 0, SOUTH, EAST, WEST, N_ORIENT };

template <class T>
struct LNode {
  T item;
  LNode<T> *next;
  LNode<T> *prev;
};

// Implement queue interface using linked list
template <class T>
class Queue {
 private:
  uint16_t num = 0;

  LNode<T> *front;
  LNode<T> *back;

 public:
  uint16_t push(T item);

  T pop();

  uint16_t size() { return num; }
};

template <class T>
uint16_t Queue<T>::push(T item) {
  if (num > 0) {
    back->next = new LNode<T>{item, nullptr, nullptr};
    back = back->next;
  } else {
    front = new LNode<T>{item, nullptr, nullptr};
    back = front;
  }

  return ++num;
}

template <class T>
T Queue<T>::pop() {
  LNode<T> *n = front;
  num--;

  if (n > 0) {
    front = front->next;
  } else {
    front = nullptr;
    back = nullptr;
  }

  T item = n->item;
  delete n;

  return item;
}

// Linked list implementation based off of in-class code
template <class T>
class LinkedList {
 public:
  LinkedList();
  ~LinkedList();

  LNode<T> *find(const T &item);

  void insert(const T &item);
  void remove(const T &item);

  void removeNode(LNode<T> *node);
  void removeFront();
  void removeBack();

  LNode<T> *getFront();

  uint16_t size();

 private:
  LNode<T> *first;
  LNode<T> *last;
  uint16_t listSize;
};

template <class T>
LinkedList<T>::LinkedList() {
  first = nullptr;
  last = nullptr;
  listSize = 0;
}

template <class T>
LinkedList<T>::~LinkedList() {
  while (first != nullptr) {
    removeFront();
  }
}

template <class T>
LNode<T> *LinkedList<T>::find(const T &item) {
  LNode<T> *node = first;

  while (node != nullptr && node->item != item) {
    node = node->next;
  }

  return node;  // nullptr if item not found
}

template <class T>
LNode<T> *LinkedList<T>::getFront() {
  return first;
}

template <class T>
void LinkedList<T>::insert(const T &item) {
  LNode<T> *node = new LNode<T>{item, nullptr, first};

  if (first != nullptr) {
    first->prev = node;
  } else {
    last = node;
  }

  first = node;
  listSize++;
}

template <class T>
void LinkedList<T>::remove(const T &item) {
  LNode<T> *node = find(item);

  if (node != nullptr) {
    removeNode(node);
  }
}

template <class T>
void LinkedList<T>::removeNode(LNode<T> *node) {
  if (node == first) {
    removeFront();
  } else if (node == last) {
    removeBack();
  } else {
    node->prev->next = node->next;
    node->next->prev = node->prev;

    delete node;
    listSize--;
  }
}

template <class T>
void LinkedList<T>::removeFront() {
  LNode<T> *toDelete = first;

  if (first != last) {
    first->next->prev = nullptr;
  } else {
    last = nullptr;
  }

  first = first->next;

  delete toDelete;
  listSize--;
}

template <class T>
void LinkedList<T>::removeBack() {
  LNode<T> *toDelete = last;

  if (first != last) {
    last->prev->next = nullptr;
  } else {
    first = nullptr;
  }

  last = last->prev;

  delete toDelete;
  listSize--;
}

template <class T>
uint16_t LinkedList<T>::size() {
  return listSize;
}

uint16_t genNeonColor();

// Data payloads are stored here to stop any circular dependency issues

struct StatePayload {
  uint8_t state;
};

struct PlayerPayload {
  uint8_t id;
  uint16_t x;
  uint16_t y;
};

#define INVALID_JID 255  // We will never have 255 ids so this won't happen

struct MapPayload {
  uint8_t id;             // Guaranteed to be > 0;
  uint8_t neighbours[4];  // TODO: Don't hardcode this value
  uint16_t x, y;
};