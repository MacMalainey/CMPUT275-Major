#pragma once

struct Point {
  uint16_t x;
  uint16_t y;
};

enum Orientation {
  NORTH = 0, SOUTH, EAST, WEST, N_ORIENT
};

template<class T>
struct LNode {
  T item;
  LNode<T> *next;
};

// Implement queue interface using linked list
template<class T>
class Queue {
 private:
  uint16_t num = 0;

  LNode<T> *front;
  LNode<T> *back;

 public:
  uint16_t push(T item) {
      if (num > 0) {
          back->next = new LNode<T>{item, nullptr};
          back = back->next;
      } else {
          front = new LNode<T>{item, nullptr};
          back = front;
      }

      return ++num;
  }

  T pop() {
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

  uint16_t size() { return num; }
};

uint16_t genNeonColor();
