

template<class T>
struct LNode {
    T item;
    LNode<T> * next;
};

// Implement queue interface using linked list
template<class T>
class Queue {
    private:

    uint16_t num = 0;

    LNode<T>* front;
    LNode<T>* back;

    public:
    uint16_t push(T item) {
        if (num > 0) {
            back->next = new LNode<T>{item, NULL};
            back = back->next;
        } else {
            front = new LNode<T>{item, NULL};
            back = front;
        }

        return ++num;
    }

    T pop() {
        LNode<T>* n = front;
        num--;

        if (n > 0) {
            front = front->next;
        } else {
            front = NULL;
            back = NULL;
        }

        T item = n->item;
        delete n;

        return item;

    }

    uint16_t size() {return num;}

};