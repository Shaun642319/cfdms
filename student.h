#ifndef STUDENT_H
#define STUDENT_H

typedef struct {
    char id[20];
    char firstName[50];
    char lastName[50];
    char username[50];
    char email[50];
    char password[20];
    double balance;
} Student;

// Dashboard
void studentMenu(Student *loggedInStudent);

// Features
void viewProfile(Student *s);
void updateProfile(Student *s);
void placeOrder(Student *s);
void viewOrderHistory(Student *s);
void topUpBalance(Student *s);

#endif
