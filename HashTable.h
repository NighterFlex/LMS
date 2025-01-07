#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Book.h"
#include <iostream>
using namespace std;

class HashTable {
private:
    static const int TABLE_SIZE = 10;
    BookNode* table[TABLE_SIZE];

    int hashISBN(const string& ISBN) {
        int hashValue = 0;
        for (char c : ISBN) {
            hashValue += c;
        }
        return hashValue % TABLE_SIZE;
    }

public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    void insert(Book book) {
        int index = hashISBN(book.ISBN);
        BookNode* newNode = new BookNode(book);
        newNode->next = table[index];
        table[index] = newNode;
    }

    BookNode* searchByISBN(const string& ISBN) {
        int index = hashISBN(ISBN);
        BookNode* current = table[index];

        while (current != nullptr) {
            if (current->book.ISBN == ISBN) {
                return current;
            }
            current = current->next;
        }

        return nullptr;
    }

    void deleteBook(const string& ISBN) {
        int index = hashISBN(ISBN);
        BookNode* current = table[index];
        BookNode* previous = nullptr;

        while (current != nullptr) {
            if (current->book.ISBN == ISBN) {
                if (previous == nullptr) {
                    // The node to delete is the head of the linked list
                    table[index] = current->next;
                }
                else {
                    // The node to delete is in the middle or end
                    previous->next = current->next;
                }
                delete current;
                cout << "Book with ISBN " << ISBN << " deleted from hash table." << endl;
                return;
            }
            previous = current;
            current = current->next;
        }

        cout << "Book with ISBN " << ISBN << " not found in hash table." << endl;
    }

    void display() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            BookNode* current = table[i];
            while (current != nullptr) {
                cout << "ISBN: " << current->book.ISBN << ", Title: " << current->book.title
                    << ", Author: " << current->book.author << endl;
                current = current->next;
            }
        }
    }
};

#endif