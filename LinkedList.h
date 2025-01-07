#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Book.h"
#include <iostream>
using namespace std;

class LinkedList {
private:
    BookNode* head;

public:
    LinkedList() : head(nullptr) {}

    void insert(Book book) {
        BookNode* newNode = new BookNode(book);
        if (head == nullptr) {
            head = newNode;
        }
        else {
            BookNode* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void deleteBook(const string& title) {
        if (head == nullptr) return;

        if (head->book.title == title) {
            BookNode* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        BookNode* temp = head;
        while (temp->next && temp->next->book.title != title) {
            temp = temp->next;
        }

        if (temp->next) {
            BookNode* bookToDelete = temp->next;
            temp->next = temp->next->next;
            delete bookToDelete;
        }
    }

    void display() {
        BookNode* temp = head;
        while (temp) {
            cout << "Title: " << temp->book.title << ", Author: " << temp->book.author
                << ", ISBN: " << temp->book.ISBN << ", Availability: "
                << (temp->book.availability ? "Available" : "Issued") << endl;
            temp = temp->next;
        }
    }

    BookNode* getHead() { return head; }
};

#endif