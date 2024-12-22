#include <iostream>
#include "Player.cpp"
using namespace std;

// Linked List Node for Queue
struct QueueNode {
    Player data;
    QueueNode* next;

    QueueNode(Player p) : data(p), next(nullptr) {}
};

// Queue Implementation
class LinkedListQueue {
private:
    QueueNode* frontNode; // Pointer to the front node
    QueueNode* rearNode;  // Pointer to the rear node
    int size;             // To track the size of the queue

public:
    LinkedListQueue() : frontNode(nullptr), rearNode(nullptr), size(0) {}

    // Enqueue: Add a player to the rear of the queue
    void push(Player p) {
        QueueNode* newNode = new QueueNode(p);
        if (rearNode == nullptr) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        size++;
    }

    // Dequeue: Remove a player from the front of the queue (does not return anything)
    void pop() {
        if (empty()) {
            throw std::runtime_error("Queue is empty. Cannot pop.");
        }
        QueueNode* temp = frontNode;
        frontNode = frontNode->next;
        if (frontNode == nullptr) {
            rearNode = nullptr; // Queue is now empty
        }
        delete temp;
        size--;
    }

    // Front: Get the player at the front of the queue
    Player& front() {
        if (empty()) {
            throw std::runtime_error("Queue is empty. Cannot access front.");
        }
        return frontNode->data;
    }

    // Back: Get the player at the rear of the queue
    Player& back() {
        if (empty()) {
            throw std::runtime_error("Queue is empty. Cannot access back.");
        }
        return rearNode->data;
    }

    // Is Empty: Check if the queue is empty
    bool empty() const {
        return frontNode == nullptr;
    }

    // Size: Get the current size of the queue
    int sizeQueue() const {
        return size;
    }

    // Destructor: Clean up memory
    ~LinkedListQueue() {
        while (!empty()) {
            pop();
        }
    }
};
