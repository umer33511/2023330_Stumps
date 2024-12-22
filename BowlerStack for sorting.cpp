#include <iostream>
using namespace std;

struct BowlerStackNode {
    string name;
    int wickets;
    int runsConceded;
    int ballsBowled;
    BowlerStackNode* next;

    BowlerStackNode(string n, int w, int r, int b)
        : name(n), wickets(w), runsConceded(r), ballsBowled(b), next(nullptr) {}
};

class BowlerStack {
public:
    BowlerStackNode* top;

    BowlerStack() : top(nullptr) {}

    ~BowlerStack() {
        while (!isEmpty()) {
            BowlerStackNode* temp = pop();
            delete temp;
        }
    }

    void push(string name, int wickets, int runsConceded, int ballsBowled) {
        BowlerStackNode* newNode = new BowlerStackNode(name, wickets, runsConceded, ballsBowled);
        newNode->next = top;
        top = newNode;
    }

    BowlerStackNode* pop() {
        if (!top) return nullptr;
        BowlerStackNode* temp = top;
        top = top->next;
        return temp; // Caller must delete this if necessary
    }

    bool isEmpty() {
        return top == nullptr;
    }

    void sortedInsert(BowlerStackNode* node) {
        if (!top || node->wickets > top->wickets) {
            node->next = top;
            top = node;
        } else {
            BowlerStackNode* temp = pop();
            sortedInsert(node);
            push(temp->name, temp->wickets, temp->runsConceded, temp->ballsBowled);
            delete temp; // Free the temporary node
        }
    }

    void sortStack() {
        if (!top) return; // Correct termination condition
        BowlerStackNode* temp = pop();
        sortStack();
        sortedInsert(temp);
    }
};
