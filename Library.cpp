
#include "LinkedList.h"
#include "HashTable.h"
#include "BinarySearchTree.h"
#include "Queue.h"
#include "Stack.h"
#include "User.h"
#include "Librarian.h"
#include <fstream>
#include <iostream>
using namespace std;

class Library {
private:
    LinkedList linkedList;
    HashTable hashTable;
    BinarySearchTree bst;
    Queue issueQueue;
    Stack undoStack;

    User users[max_size];
    int userCount;

public:
    Library() : userCount(0) {
        loadUsers();
        loadBooks();
    }

    ~Library() {
        saveUsers();
        saveBooks();
    }

    // File handling for users
    void loadUsers() {
        ifstream file("users.txt");
        if (!file) return;

        string name, id, role, issuedBook;
        while (file >> name >> id >> role >> issuedBook) {
            users[userCount] = User(name, id, role);
            users[userCount].issuedBook = issuedBook;
            userCount++;
        }
        file.close();
    }

    void saveUsers() {
        ofstream file("users.txt");
        for (int i = 0; i < userCount; i++) {
            file << users[i].name << " " << users[i].id << " " << users[i].role << " " << users[i].issuedBook << endl;
        }
        file.close();
    }

    // File handling for books
    void loadBooks() {
        ifstream file("books.txt");
        if (!file) return;

        string title, author, ISBN;
        bool availability;
        while (file >> title >> author >> ISBN >> availability) {
            Book newBook(title, author, ISBN);
            newBook.availability = availability;
            addBook(newBook);
        }
        file.close();
    }

    void saveBooks() {
        ofstream file("books.txt");
        BookNode* temp = linkedList.getHead();
        while (temp != nullptr) {
            file << temp->book.title << " " << temp->book.author << " " << temp->book.ISBN << " " << temp->book.availability << endl;
            temp = temp->next;
        }
        file.close();
    }

    // User management
    void addUser(string name, string id, string role) {
        if (userCount >= max_size) {
            cout << "User limit reached!" << endl;
            return;
        }
        users[userCount] = User(name, id, role);
        userCount++;
        cout << "User added: " << name << endl;
    }

    void displayUsers() {
        if (userCount == 0) {
            cout << "No users found!" << endl;
            return;
        }
        for (int i = 0; i < userCount; i++) {
            cout << "Name: " << users[i].name << ", ID: " << users[i].id << ", Role: " << users[i].role;
            if (!users[i].issuedBook.empty()) {
                cout << ", Issued Book: " << users[i].issuedBook;
            }
            cout << endl;
        }
    }

    void updateUser(string id, string newName, string newRole) {
        for (int i = 0; i < userCount; i++) {
            if (users[i].id == id) {
                users[i].name = newName;
                users[i].role = newRole;
                cout << "User updated: " << users[i].name << endl;
                return;
            }
        }
        cout << "User not found with ID: " << id << endl;
    }

    void deleteUser(string id) {
        for (int i = 0; i < userCount; i++) {
            if (users[i].id == id) {
                cout << "User deleted: " << users[i].name << endl;
                // Shift all users after the deleted user one position to the left
                for (int j = i; j < userCount - 1; j++) {
                    users[j] = users[j + 1];
                }
                userCount--;
                return;
            }
        }
        cout << "User not found with ID: " << id << endl;
    }

    User* findUserById(string id) {
        for (int i = 0; i < userCount; i++) {
            if (users[i].id == id) {
                return &users[i];
            }
        }
        return nullptr;
    }

    // Issue a book to a user
    void issueBookToUser(string ISBN, string userId) {
        BookNode* book = hashTable.searchByISBN(ISBN);
        User* user = findUserById(userId);

        if (!book || !user) {
            cout << "Book or user not found!" << endl;
            return;
        }

        if (!book->book.availability) {
            cout << "Book is already issued!" << endl;
            return;
        }

        if (!user->issuedBook.empty()) {
            cout << "User already has a book issued!" << endl;
            return;
        }

        book->book.availability = false;
        user->issuedBook = ISBN;
        issueQueue.enqueue(book);
        cout << "Book issued to user: " << user->name << endl;
    }

    void searchByISBN(const string& ISBN) {
        BookNode* foundBook = hashTable.searchByISBN(ISBN);
        if (foundBook != nullptr) {
            cout << "Book found! ISBN: " << foundBook->book.ISBN << ", Title: " << foundBook->book.title
                << ", Author: " << foundBook->book.author << ", Availability: "
                << (foundBook->book.availability ? "Available" : "Issued") << endl;
        }
        else {
            cout << "Book not found with ISBN: " << ISBN << endl;
        }
    }

    // Return a book from a user
    void returnBookFromUser(string userId) {
        User* user = findUserById(userId);
        if (!user || user->issuedBook.empty()) {
            cout << "User has no book to return!" << endl;
            return;
        }

        BookNode* book = hashTable.searchByISBN(user->issuedBook);
        if (!book) {
            cout << "Book not found!" << endl;
            return;
        }

        book->book.availability = true;
        user->issuedBook = "";
        cout << "Book returned by user: " << user->name << endl;
    }

    // Add a book to the library
    void addBook(Book book) {
        linkedList.insert(book);  // Add to linked list
        bst.insert(book);         // Add to BST
        hashTable.insert(book);   // Add to hash table

        Action* addAction = new Action(ADD, book);
        undoStack.push(addAction);

        cout << "Book added: " << book.title << endl;
    }

    // Delete a book from the library
    void deleteBook(const string& title) {
        BookNode* bookNode = linkedList.getHead();
        while (bookNode && bookNode->book.title != title) {
            bookNode = bookNode->next;
        }

        if (bookNode) {
            linkedList.deleteBook(title);
            bst.deleteBook(title);
            hashTable.deleteBook(title);

            Action* deleteAction = new Action(DELETE, bookNode->book);
            undoStack.push(deleteAction);

            cout << "Book deleted: " << title << endl;
        }
        else {
            cout << "Book not found!" << endl;
        }
    }

    // Display all books in the library
    void displayBooks() {
        linkedList.display();
    }

    // Display all issued books
    void displayIssuedBooks() {
        issueQueue.display();
    }

    // Undo the last action
    void undo() {
        if (undoStack.isEmpty()) {
            cout << "No actions to undo." << endl;
            return;
        }

        Action* lastAction = undoStack.pop();
        if (lastAction->type == ADD) {
            deleteBook(lastAction->book.title);
            cout << "Undo: Book '" << lastAction->book.title << "' removed." << endl;
        }
        else if (lastAction->type == DELETE) {
            addBook(lastAction->book);
            cout << "Undo: Book '" << lastAction->book.title << "' added back." << endl;
        }

        delete lastAction;
    }

    // Librarian login
    bool librarianLogin() {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        ifstream file("librarians.txt");
        if (!file) {
            cout << "No librarians found. Please sign up." << endl;
            return false;
        }

        string storedUsername, storedPassword;
        while (file >> storedUsername >> storedPassword) {
            if (storedUsername == username && storedPassword == password){
                cout << "Login successful! Welcome, " << username << "." << endl;
                file.close();
                return true;
                }
        }

        cout << "Invalid username or password." << endl;
            file.close();
            return false;
    }

    // Librarian signup
    void librarianSignup() {
        string username, password;
        cout << "Enter a username: ";
        cin >> username;
        cout << "Enter a password: ";
        cin >> password;

        ofstream file("librarians.txt", ios::app);
        file << username << " " << password << endl;
        file.close();

        cout << "Signup successful! You can now log in." << endl;
    }

    void run() {
        int choice;
        string title, author, ISBN, name, id, role;

        // Librarian login/signup
        do {
            cout << "\n\nLibrary Management System\n";
            cout << "1. Login\n";
            cout << "2. Signup\n";
            cout << "3. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                if (librarianLogin()) {
                    // Main menu after successful login
                    do {
                        cout << "\n\nMain Menu\n";
                        cout << "1. User Operations\n";
                        cout << "2. Book Operations\n";
                        cout << "3. Logout\n";
                        cout << "Enter your choice: ";
                        cin >> choice;

                        switch (choice) {
                        case 1:
                            // User operations menu
                            do {
                                cout << "\n\nUser Operations\n";
                                cout << "1. Add User\n";
                                cout << "2. Update User\n";
                                cout << "3. Delete User\n";
                                cout << "4. Display All Users\n";
                                cout << "5. Issue Book to User\n";
                                cout << "6. Return Book from User\n";
                                cout << "7. Back to Main Menu\n";
                                cout << "Enter your choice: ";
                                cin >> choice;

                                switch (choice) {
                                case 1:
                                    cout << "Enter user name: ";
                                    cin.ignore();
                                    getline(cin, name);
                                    cout << "Enter user ID: ";
                                    cin >> id;
                                    cout << "Enter user role (student/faculty): ";
                                    cin >> role;
                                    addUser(name, id, role);
                                    break;

                                case 2:
                                    cout << "Enter user ID to update: ";
                                    cin >> id;
                                    cout << "Enter new name: ";
                                    cin.ignore();
                                    getline(cin, name);
                                    cout << "Enter new role (student/faculty): ";
                                    cin >> role;
                                    updateUser(id, name, role);
                                    break;

                                case 3:
                                    cout << "Enter user ID to delete: ";
                                    cin >> id;
                                    deleteUser(id);
                                    break;

                                case 4:
                                    displayUsers();
                                    break;

                                case 5:
                                    cout << "Enter ISBN of the book to issue: ";
                                    cin >> ISBN;
                                    cout << "Enter user ID: ";
                                    cin >> id;
                                    issueBookToUser(ISBN, id);
                                    break;

                                case 6:
                                    cout << "Enter user ID: ";
                                    cin >> id;
                                    returnBookFromUser(id);
                                    break;

                                case 7:
                                    cout << "Returning to main menu..." << endl;
                                    break;

                                default:
                                    cout << "Invalid choice! Please try again." << endl;
                                }

                                cout << "\nPress Enter to continue...";
                                cin.ignore();
                                cin.get();

                                system("CLS");

                            } while (choice != 7);
                            break;

                        case 2:
                            // Book operations menu
                            do {
                                cout << "\n\nBook Operations\n";
                                cout << "1. Add Book\n";
                                cout << "2. Delete Book\n";
                                cout << "3. Display All Books\n";
                                cout << "4. Display Issued Books\n";
                                cout << "5. Search Book by ISBN\n";
                                cout << "6. Search Book by Title\n";
                                cout << "7. Undo Last Action\n";
                                cout << "8. Back to Main Menu\n";
                                cout << "Enter your choice: ";
                                cin >> choice;

                                switch (choice) {
                                case 1:
                                    cout << "Enter book title: ";
                                    cin.ignore();
                                    getline(cin, title);
                                    cout << "Enter author name: ";
                                    getline(cin, author);
                                    cout << "Enter ISBN: ";
                                    cin >> ISBN;
                                    addBook(Book(title, author, ISBN));
                                    break;

                                case 2:
                                    cout << "Enter book title to delete: ";
                                    cin.ignore();
                                    getline(cin, title);
                                    deleteBook(title);
                                    break;

                                case 3:
                                    displayBooks();
                                    break;

                                case 4:
                                    displayIssuedBooks();
                                    break;

                                case 5:
                                    cout << "Enter ISBN of the book to search: ";
                                    cin >> ISBN;
                                    searchByISBN(ISBN);
                                    break;

                                case 6:
                                    cout << "Enter book title to search: ";
                                    cin.ignore();
                                    getline(cin, title);
                                    if (bst.search(title)) {
                                        cout << "Book found!" << endl;
                                    }
                                    else {
                                        cout << "Book not found!" << endl;
                                    }
                                    break;

                                case 7:
                                    undo();
                                    break;

                                case 8:
                                    cout << "Returning to main menu..." << endl;
                                    break;

                                default:
                                    cout << "Invalid choice! Please try again." << endl;
                                }

                                cout << "\nPress Enter to continue...";
                                cin.ignore();
                                cin.get();

                                system("CLS");

                            } while (choice != 8);
                            break;

                        case 3:
                            cout << "Logging out..." << endl;
                            break;

                        default:
                            cout << "Invalid choice! Please try again." << endl;
                        }

                        cout << "\nPress Enter to continue...";
                        cin.ignore();
                        cin.get();

                        system("CLS");

                    } while (choice != 3);
                }
                break;

            case 2:
                librarianSignup();
                break;

            case 3:
                cout << "Exiting the system. Thank you!" << endl;
                break;

            default:
                cout << "Invalid choice! Please try again." << endl;
            }

            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();

            system("CLS");

        } while (choice != 3);
    }
};

int main() {

    Library lib;
    lib.run(); // Run the library interface
    return 0;

}