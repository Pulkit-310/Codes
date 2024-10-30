#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store book details
typedef struct Book {
    int id;
    char title[100];
    char author[50];
    int is_issued; // 0 if available, 1 if issued
    struct Book* next;
} Book;

// Function prototypes
Book* load_books();
void save_books(Book* head);
Book* create_book(int id, char* title, char* author, int is_issued);
void add_book(Book** head, int id, char* title, char* author);
void remove_book(Book** head, int id);
void issue_book(Book* head, int id);
void return_book(Book* head, int id);
void view_books(Book* head);
void free_books(Book* head);

int main() {
    Book* library = load_books();
    int choice, id;
    char title[100], author[50];

    while (1) {
        printf("\n--- Library Book Management System ---\n");
        printf("1. Add Book\n2. Remove Book\n3. Issue Book\n4. Return Book\n5. View Books\n6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter book ID: ");
                scanf("%d", &id);
                printf("Enter book title: ");
                scanf(" %[^\n]s", title);
                printf("Enter author name: ");
                scanf(" %[^\n]s", author);
                add_book(&library, id, title, author);
                break;

            case 2:
                printf("Enter book ID to remove: ");
                scanf("%d", &id);
                remove_book(&library, id);
                break;

            case 3:
                printf("Enter book ID to issue: ");
                scanf("%d", &id);
                issue_book(library, id);
                break;

            case 4:
                printf("Enter book ID to return: ");
                scanf("%d", &id);
                return_book(library, id);
                break;

            case 5:
                view_books(library);
                break;

            case 6:
                save_books(library);
                free_books(library);
                printf("Exiting Library Book Management System.\n");
                return 0;

            default:
                printf("Invalid choice! Please enter a valid option.\n");
        }
    }
}

// Load books from file
Book* load_books() {
    FILE* file = fopen("books.txt", "r");
    if (!file) return NULL;

    Book* head = NULL;
    int id, is_issued;
    char title[100], author[50];

    while (fscanf(file, "%d %[^\n] %[^\n] %d", &id, title, author, &is_issued) != EOF) {
        add_book(&head, id, title, author);
        head->is_issued = is_issued;
    }
    fclose(file);
    return head;
}

// Save books to file
void save_books(Book* head) {
    FILE* file = fopen("books.txt", "w");
    if (!file) return;

    Book* temp = head;
    while (temp != NULL) {
        fprintf(file, "%d\n%s\n%s\n%d\n", temp->id, temp->title, temp->author, temp->is_issued);
        temp = temp->next;
    }
    fclose(file);
}

// Create a new book node
Book* create_book(int id, char* title, char* author, int is_issued) {
    Book* new_book = (Book*)malloc(sizeof(Book));
    new_book->id = id;
    strcpy(new_book->title, title);
    strcpy(new_book->author, author);
    new_book->is_issued = is_issued;
    new_book->next = NULL;
    return new_book;
}

// Add a book to the catalog
void add_book(Book** head, int id, char* title, char* author) {
    Book* new_book = create_book(id, title, author, 0);
    new_book->next = *head;
    *head = new_book;
    printf("Book added successfully!\n");
}

// Remove a book by ID
void remove_book(Book** head, int id) {
    Book *temp = *head, *prev = NULL;

    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Book not found in library.\n");
        return;
    }
    if (prev == NULL) {
        *head = temp->next;
    } else {
        prev->next = temp->next;
    }
    free(temp);
    printf("Book removed successfully!\n");
}

// Issue a book by ID
void issue_book(Book* head, int id) {
    Book* temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            if (temp->is_issued == 1) {
                printf("Book is already issued.\n");
                return;
            }
            temp->is_issued = 1;
            printf("Book issued successfully!\n");
            return;
        }
        temp = temp->next;
    }
    printf("Book not found in library.\n");
}

// Return a book by ID
void return_book(Book* head, int id) {
    Book* temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            if (temp->is_issued == 0) {
                printf("Book is already available.\n");
                return;
            }
            temp->is_issued = 0;
            printf("Book returned successfully!\n");
            return;
        }
        temp = temp->next;
    }
    printf("Book not found in library.\n");
}

// View all books in the library
void view_books(Book* head) {
    if (head == NULL) {
        printf("Library is empty.\n");
        return;
    }
    Book* temp = head;
    printf("\n--- Library Catalog ---\n");
    while (temp != NULL) {
        printf("ID: %d | Title: %s | Author: %s | Status: %s\n", 
               temp->id, temp->title, temp->author, temp->is_issued ? "Issued" : "Available");
        temp = temp->next;
    }
}

// Free all nodes in the library
void free_books(Book* head) {
    Book* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
