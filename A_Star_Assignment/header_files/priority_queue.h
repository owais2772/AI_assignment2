//
// Created by hisha on 11/26/2023.
//

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#pragma once
#include <iostream>


template<typename T>
class PriorityNode {
public:
    T data;
    PriorityNode<T>* next;

    PriorityNode(const T& value) : data(value), next(nullptr) {
    }
};

template<typename T>
class CustomPriorityQueue {
private:
    PriorityNode<T>* head;

    bool (*compare)(T val, T other);

public:
    CustomPriorityQueue() : head(nullptr), compare(nullptr) {
    }

    // Constructor
    CustomPriorityQueue(bool (*cmp)(T, T)) : head(nullptr), compare(cmp) {
    }

    // Destructor
    ~CustomPriorityQueue() {
        while (!empty()) {
            pop();
        }
    }

    // Insert an element into the priority queue
    void push(const T& value) {
        // Check if the element already exists
        // and then check if the new element has higher priority
        // then take action according to it
        if (contains(value)) {
            PriorityNode<T>* current = head;
            PriorityNode<T>* prev = nullptr;

            // Find the existing node with the same data
            while (current != nullptr && current->data != value) {
                prev = current;
                current = current->next;
            }

            if (compare(value, current->data)) {
                // Remove the existing node with lower priority
                if (prev == nullptr) {
                    head = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
            } else {
                // Ignore the duplicate with lower priority
                return;
            }
        }

        // Insert the new element
        PriorityNode<T>* newPriorityNode = new PriorityNode<T>(value);
        if (head == nullptr || compare(value, head->data)) {
            newPriorityNode->next = head;
            head = newPriorityNode;
        } else {
            PriorityNode<T>* current = head;
            while (current->next != nullptr && !compare(value, current->next->data)) {
                current = current->next;
            }
            newPriorityNode->next = current->next;
            current->next = newPriorityNode;
        }
    }

    T top() const {
        if (head != nullptr) {
            return head->data;
        } else {
            throw std::runtime_error("Priority queue is empty");
        }
    }

    T pop() {
        T data = top();
        head = head->next;
        return data;
    }

    // Check if the priority queue is empty
    bool empty() const {
        return head == nullptr;
    }

    bool contains(T data) {
        PriorityNode<T>* current = head;

        while (current != nullptr) {
            if (current->data == data) {
                return true;
            }
            current = current->next;
        }

        return false;
    }

    int size() {
        int i = 0;
        PriorityNode<T>* current = head;
        while (current != nullptr) {
            i++;
            current = current->next;
        }
        return i;
    }
};

// int main()
// {
//     // Create a custom priority queue
//     CustomPriorityQueue<int> pq;

//     // Insert elements into the custom priority queue
//     pq.push(5);
//     pq.push(2);
//     pq.push(10);
//     pq.push(1);

//     // Print the elements in the custom priority queue
//     while (!pq.empty())
//     {
//         std::cout << pq.top() << " ";
//         pq.pop();
//     }

//     return 0;
// }


#endif //PRIORITY_QUEUE_H
