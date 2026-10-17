
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Book {
    int id;
    char title[100];
    int available;
    struct Book* next;
} Book;

typedef struct User {
    int id;
    char name[50];
    int borrowedCount;
    struct User* next;
} User;

typedef struct Borrow {
    int userId;
    int bookId;
    struct Borrow* next;
} Borrow;

Book* bookHead = NULL;
User* userHead = NULL;
Borrow* borrowHead = NULL;

// Kitap ekleme
void addBook(int id, char* title) {
    Book* newBook = (Book*)malloc(sizeof(Book));
    newBook->id = id;
    strcpy(newBook->title, title);
    newBook->available = 1;
    newBook->next = bookHead;
    bookHead = newBook;
}

// Kullanici ekleme
void addUser(int id, char* name) {
    User* newUser = (User*)malloc(sizeof(User));
    newUser->id = id;
    strcpy(newUser->name, name);
    newUser->borrowedCount = 0;
    newUser->next = userHead;
    userHead = newUser;
}

// Kitap ödünç alma
void borrowBook(int userId, int bookId) {
    Book* b = bookHead;
    while (b != NULL && b->id != bookId) b = b->next;
    if (b == NULL || b->available == 0) {
        printf("Book not available.\n");
        return;
    }
    User* u = userHead;
    while (u != NULL && u->id != userId) u = u->next;
    if (u == NULL) {
        printf("User not found.\n");
        return;
    }
    Borrow* newBorrow = (Borrow*)malloc(sizeof(Borrow));
    newBorrow->userId = userId;
    newBorrow->bookId = bookId;
    newBorrow->next = borrowHead;
    borrowHead = newBorrow;

    b->available = 0;
    u->borrowedCount += 1;
    printf("Book borrowed successfully.\n");
}

// Kitap iade
void returnBook(int bookId) {
    Borrow* prev = NULL, *curr = borrowHead;
    while (curr != NULL && curr->bookId != bookId) {
        prev = curr;
        curr = curr->next;
    }
    if (curr == NULL) {
        printf("Borrow record not found.\n");
        return;
    }
    // Kullanicinin borrowedCount'unu azalt
    User* u = userHead;
    while (u != NULL && u->id != curr->userId) u = u->next;
    if (u != NULL) u->borrowedCount--;

    // Kitabi tekrar kullanilabilir yap
    Book* b = bookHead;
    while (b != NULL && b->id != bookId) b = b->next;
    if (b != NULL) b->available = 1;

    // Borrow listesinden sil
    if (prev == NULL) borrowHead = curr->next;
    else prev->next = curr->next;
    free(curr);

    printf("Book returned successfully.\n");
}

// Tüm kitaplari göster
void viewBooks() {
    Book* b = bookHead;
    while (b != NULL) {
        printf("Book ID: %d, Title: %s, Status: %s\n", b->id, b->title, b->available ? "Available" : "Borrowed");
        b = b->next;
    }
}

// Tüm kullanicilari göster
void viewUsers() {
    User* u = userHead;
    while (u != NULL) {
        printf("User ID: %d, Name: %s, Books Borrowed: %d\n", u->id, u->name, u->borrowedCount);
        u = u->next;
    }
}

// En aktif kullaniciyi bul
void mostActiveUser() {
    User* u = userHead;
    User* top = NULL;
    int max = -1;
    while (u != NULL) {
        if (u->borrowedCount > max) {
            max = u->borrowedCount;
            top = u;
        }
        u = u->next;
    }
    if (top != NULL) {
        printf("Most Active User: %s (ID: %d) - %d books\n", top->name, top->id, top->borrowedCount);
    }
}

int main() {
    addBook(1, "The C Programming Language");
    addBook(2, "Data Structures and Algorithms");

    addUser(101, "Alice");
    addUser(102, "Bob");

    viewBooks();
    viewUsers();

    borrowBook(101, 1);
    borrowBook(102, 2);

    viewBooks();
    viewUsers();

    returnBook(1);
    viewBooks();
    viewUsers();

    mostActiveUser();

    return 0;
}

