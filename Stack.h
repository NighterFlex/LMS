#ifndef STACK_H
#define STACK_H

#include "Book.h"
#include <iostream>
using namespace std;

enum ActionType { ADD, DELETE };

struct Action {
    ActionType type;
    Book book;

    Action(ActionType t, Book b) : type(t), book(b) {}
};

class Stack {
private:
    Action* arr[max_size];
    int top;

public:
    Stack() : top(-1) {
        for (int i = 0; i < max_size; i++) {
            arr[i] = nullptr;
        }
    }

    bool isEmpty() {
        return top == -1;
    }

    bool isFull() {
        return top == max_size - 1;
    }

    void push(Action* action) {
        if (isFull()) {
            cout << "Stack Overflow" << endl;
        }
        else {
            top++;
            arr[top] = action;
        }
    }

    Action* pop() {
        if (isEmpty()) {
            cout << "Stack Underflow" << endl;
            return nullptr;
        }
        else {
            Action* action = arr[top];
            arr[top] = nullptr;
            top--;
            return action;
        }
    }

    void display() {
        for (int i = top; i >= 0; i--) {
            cout << (arr[i]->type == ADD ? "Add " : "Delete ")
                << "Book - Title: " << arr[i]->book.title
                << ", ISBN: " << arr[i]->book.ISBN << endl;
        }
    }
};

#endif