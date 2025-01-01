#include <iostream>
#include <string>
using namespace std;

struct Book {
    string title;
    string author;
    string ISBN;
    bool availability;

    Book() {
        title = "\0";
        author = "\0";
        ISBN = "\0";
        availability = true;
    }

    Book(string t, string a, string i) {
        title = t;
        author = a;
        ISBN = i;
        availability = true;
    }
};

// Node for linked list
struct BookNode {
    Book book;
    BookNode* next;

    BookNode(Book& b) {
        book = b;
        next = nullptr;
    }
};

class Queue {
private:
    int front;
    int rear;
    BookNode* arr[10];  

public:
    Queue(){
    	front = -1;
		rear = -1; 
	}

    bool isEmpty() { 
    
	return front == -1 && rear == -1;
	}
    bool isFull() {
	return rear == 9;
	}

    void enqueue(BookNode* val) {
        if (isFull()) {
            cout << "Issuing queue is full! :(" << endl;
            return;
        }
        if (isEmpty()) {
            rear = 0;
            front = 0;
        }
        else {
            rear++;
        }
        arr[rear] = val;
    }

    BookNode* dequeue() {
        if (isEmpty()) {
            cout << "Issuing queue is empty! :D" << endl;
            return nullptr;
        }
        BookNode* x = arr[front];
        if (front == rear) {
            front = -1;
            rear = -1;
        }
        else {
            front++;
        }
        return x;
    }

    int count() { return rear - front + 1; }

    void display() {
        if (isEmpty()) {
            cout << "No books in the issuing queue." << endl;
            return;
        }
        for (int i = front; i <= rear; i++) {
            cout << "Title: " << arr[i]->book.title<< ", ISBN: " << arr[i]->book.ISBN << endl;
        }
    }
};

class Library {
private:
    BookNode* head;      
    Queue issueQueue;   

public:
    Library(){
	head = nullptr;
}

 
    void addBook(string& title, string& author,string& ISBN) {
        Book newBook(title, author, ISBN);  // Book object
        BookNode* newNode = new BookNode(newBook); 
        
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
        cout << ".o. Book added: " << title << endl;
    }

  
    void issueBook(const string& ISBN) {
        BookNode* temp = head;
        while (temp) {
            if (temp->book.ISBN == ISBN) {
                if (!temp->book.availability) {
                    cout << "Book is already issued! :|" << endl;
                    return;   //cuz void is a bish, it too needs sumn in return T-T
                }
                temp->book.availability = false;  // Mark book as issued
                issueQueue.enqueue(temp);  //add book to za queueueuuueue
                cout << "Book issued: " << temp->book.title << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "Book not found!" << endl;
    }

    void returnBook() {
        BookNode* returnedBook = issueQueue.dequeue();
        if (!returnedBook) return;

        returnedBook->book.availability = true;  // Mark the book as available
        cout << "Book returned: " << returnedBook->book.title << endl;
    }

    // Function to display all issued books
    void displayIssuedBooks() {
        cout << "Issued Books:" << endl;
        issueQueue.display();
    }

    // Function to display all books in the library
    void displayBooks() {
        if (!head) {
            cout << "No books in the library. :(" << endl;
            return;
        }
        BookNode* temp = head;
        while (temp) {
            cout << "Title: " << temp->book.title<< ", Author: " << temp->book.author<< ", ISBN: " << temp->book.ISBN<< ", Availability: ";
            if(temp->book.availability){
            	cout<<"Available!"<<endl;
			}
			else{
				cout<<"Issued :("<<endl;
			}
            temp = temp->next;
        }
    }
};

int main() {
    Library lib;
    int choice;
    string title, author, ISBN;

    do {

        cout << "\n\nLibrary Management System\n";
        cout << "1. Add Book\n";
        cout << "2. Issue Book\n";
        cout << "3. Return Book\n";
        cout << "4. Display All Books\n";
        cout << "5. Display Issued Books\n";
        cout << "6. Exit\n";
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
            lib.addBook(title, author, ISBN);
            break;

        case 2:
            cout << "Enter ISBN of the book to issue: ";
            cin >> ISBN;
            lib.issueBook(ISBN);
            break;

        case 3:
            lib.returnBook();
            break;

        case 4:
            lib.displayBooks();
            break;

        case 5:
            lib.displayIssuedBooks();
            break;

        case 6:
            cout << "Exiting the system. Thank you! :DDDD" << endl;
            break;

        default:
            cout << "Invalid choice! Please try again. T-T" << endl;
        }
        
         cout << "\nPress Enter to continue...";
        //cin.ignore();  
        cin.get();
        
        system("CLS");

    } while (choice != 6);

    return 0;
}
