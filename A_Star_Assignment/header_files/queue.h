//
// Created by hisha on 11/26/2023.
//

#ifndef QUEUE_H
#define QUEUE_H


template<typename T>
struct QueueNode {
    T data;
    QueueNode<T>* next;

    QueueNode(const T& value) : data(value), next(nullptr) {
    }
};

template<typename T>
class Queue {
private:
    QueueNode<T>* front;
    QueueNode<T>* rear;

public:
    Queue() : front(nullptr), rear(nullptr) {
    }

    void insert(T& data) {
        QueueNode<T>* newNode = new QueueNode<T>(data);

        if (isEmpty()) {
            front = newNode;
            rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    T pop() {
        if (isEmpty()) {
            return nullptr;
        }

        T data = front->data;
        QueueNode<T>* temp = front;
        front = front->next;
        delete temp;

        if (front == nullptr) {
            rear = nullptr;
        }

        return data;
    }

    bool isEmpty() {
        return (front == nullptr && rear == nullptr);
    }

    T top() {
        if (isEmpty()) {
            return nullptr;
        }

        return front->data;
    }

    bool contains(T data) {
        QueueNode<T>* current = front;

        while (current != nullptr) {
            if (current->data == data) {
                return true;
            }

            current = current->next;
        }

        return false;
    }
};

#endif //QUEUE_H
