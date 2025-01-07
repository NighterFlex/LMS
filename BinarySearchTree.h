#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include "Book.h"

class BinarySearchTree {
private:
    BookNode* root;

    BookNode* insertHelper(BookNode* node, Book book) {
        if (node == nullptr) {
            return new BookNode(book);
        }

        if (book.title < node->book.title) {
            node->left = insertHelper(node->left, book);
        }
        else {
            node->right = insertHelper(node->right, book);
        }

        return node;
    }

    BookNode* searchHelper(BookNode* node, const string& title) {
        if (node == nullptr || node->book.title == title) {
            return node;
        }

        if (title < node->book.title) {
            return searchHelper(node->left, title);
        }

        return searchHelper(node->right, title);
    }

    BookNode* deleteHelper(BookNode* node, const string& title) {
        if (node == nullptr) {
            return node;
        }

        if (title < node->book.title) {
            node->left = deleteHelper(node->left, title);
        }
        else if (title > node->book.title) {
            node->right = deleteHelper(node->right, title);
        }
        else {
            if (node->left == nullptr) {
                BookNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                BookNode* temp = node->left;
                delete node;
                return temp;
            }

            BookNode* temp = minValueNode(node->right);
            node->book = temp->book;
            node->right = deleteHelper(node->right, temp->book.title);
        }
        return node;
    }

    BookNode* minValueNode(BookNode* node) {
        BookNode* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

public:
    BinarySearchTree() : root(nullptr) {}

    void insert(Book book) {
        root = insertHelper(root, book);
    }

    BookNode* search(const string& title) {
        return searchHelper(root, title);
    }

    void deleteBook(const string& title) {
        root = deleteHelper(root, title);
    }
};

#endif